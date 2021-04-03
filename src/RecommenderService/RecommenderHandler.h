#ifndef MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H
#define MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>
#include <mongoc.h>
#include <bson/bson.h>

#include "../../gen-cpp/RecommenderService.h"
#include "../../gen-cpp/MovieInfoService.h"
#include "../../gen-cpp/UserLikesService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class RecommenderServiceHandler : public RecommenderServiceIf {
 public:
  RecommenderServiceHandler(
          mongoc_client_pool_t *,
		  ClientPool<ThriftClient<MovieInfoServiceClient>> *,
		  ClientPool<ThriftClient<UserLikesServiceClient>> *) ;
  ~RecommenderServiceHandler() override=default;

  void GetRecommendations(std::vector<std::string>& _return, const int64_t user) override;
  void UploadRecommendations(const int64_t user_id, const std::vector<std::string> & movie_id) override;
 private:
  mongoc_client_pool_t *_mongodb_client_pool;
  ClientPool<ThriftClient<MovieInfoServiceClient>> *_movie_info_client_pool;
  ClientPool<ThriftClient<UserLikesServiceClient>> *_user_likes_client_pool;
};

// Constructor
RecommenderServiceHandler::RecommenderServiceHandler(
        mongoc_client_pool_t *mongodb_client_pool,
		ClientPool<ThriftClient<MovieInfoServiceClient>> *movie_info_client_pool,
		ClientPool<ThriftClient<UserLikesServiceClient>> *user_likes_client_pool) {

     // Storing the clientpool
     _mongodb_client_pool = mongodb_client_pool;
     _movie_info_client_pool = movie_info_client_pool;
	 _user_likes_client_pool = user_likes_client_pool;
}

// Remote Procedure "UploadRecommendations"
void RecommenderServiceHandler::UploadRecommendations(const int64_t user_id, const std::vector<std::string> & movie_id){

    // Get mongo client
    mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);

    if (!mongodb_client) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_MONGODB_ERROR;
      se.message = "Failed to pop a client from MongoDB pool";
      throw se;
    }

    // Get mongo collection
    auto collection = mongoc_client_get_collection(
        mongodb_client, "recommender", "recommender");

    if (!collection) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_MONGODB_ERROR;
      se.message = "Failed to create collection user from DB recommender";
      mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
      throw se;
    }

    // Check if the recommendations for this user already exist in the database
      bson_t *query = bson_new();
      BSON_APPEND_UTF8(query, "user_id", std::to_string(user_id).c_str());

      mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
      const bson_t *doc;
      bool found = mongoc_cursor_next(cursor, &doc);

      if (found) {
        ServiceException se;
        se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
        se.message = "User ID " + std::to_string(user_id) + " already existed in MongoDB Recommender";
        mongoc_cursor_destroy(cursor);
        mongoc_collection_destroy(collection);
        mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
        throw se;
      } else {
        bson_t *new_doc = bson_new();
            BSON_APPEND_UTF8(new_doc, "user_id", std::to_string(user_id).c_str());
            BSON_APPEND_UTF8(new_doc, "movie_ids", movie_id.front().c_str());
            bson_error_t error;

            bool plotinsert = mongoc_collection_insert_one (
                collection, new_doc, nullptr, nullptr, &error);

            if (!plotinsert) {
              LOG(error) << "Failed to insert recommendations for " << std::to_string(user_id) << " to MongoDB: " << error.message;
              ServiceException se;
              se.errorCode = ErrorCode::SE_MONGODB_ERROR;
              se.message = error.message;
              bson_destroy(new_doc);
              mongoc_cursor_destroy(cursor);
              mongoc_collection_destroy(collection);
              mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
              throw se;
            }
            bson_destroy(new_doc);

      }

    // cleanup
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
}

// Remote Procedure "GetRecommendations"
void RecommenderServiceHandler::GetRecommendations(std::vector<std::string>& _return, const int64_t user){
    
    // Get recommended movie ids for this user
    std::vector<std::string> movie_ids;
    movie_ids.push_back("abcd");
    movie_ids.push_back("efgh");

    // Get the movie info service client pool
    auto movie_info_client_wrapper = _movie_info_client_pool->Pop();
    if (!movie_info_client_wrapper) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
      se.message = "Failed to connect to movie-info-service";
      throw se;
    }
    auto movie_info_client = movie_info_client_wrapper->GetClient();

    // Call the remote procedure : GetMoviesByIds
    std::vector<std::string> _return_movies;
    try {
      movie_info_client->GetMoviesByIds(_return_movies, movie_ids);
      _return = _return_movies;
    } catch (...) {
      _movie_info_client_pool->Push(movie_info_client_wrapper);
      LOG(error) << "Failed to send call GetMoviesByIds to movie-info-client";
      throw;
    }
    _movie_info_client_pool->Push(movie_info_client_wrapper);
	
	// Get the user likes service client pool
    auto user_likes_client_wrapper = _user_likes_client_pool->Pop();
    if (!user_likes_client_wrapper) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
      se.message = "Failed to connect to user-likes-service";
      throw se;
    }
    auto user_likes_client = user_likes_client_wrapper->GetClient();

    // Call the remote procedure : GetMovieLikesByIds
    std::vector<int64_t> _return_movie_likes;
    try {
		// Doesn't currently change output, call is simply made to check if error occurs
      user_likes_client->GetMovieLikesByIds(_return_movie_likes, movie_ids);
    } catch (...) {
      _user_likes_client_pool->Push(user_likes_client_wrapper);
      LOG(error) << "Failed to send call GetMovieLikesByIds to user-likes-client";
      throw;
    }
    _user_likes_client_pool->Push(user_likes_client_wrapper);
	
	// Call the remote procedure : LikeDislikeMovie
	std::string user_id = "123";
	std::string user_movie_id = "xyz";
	bool user_like_dislike = false;
    try {
		// Doesn't currently do anything, call is simply made to check if error occurs
      user_likes_client->LikeDislikeMovie(user_id, user_movie_id, user_like_dislike);
    } catch (...) {
      _user_likes_client_pool->Push(user_likes_client_wrapper);
      LOG(error) << "Failed to send call LikeDislikeMovie to user-likes-client";
      throw;
    }
    _user_likes_client_pool->Push(user_likes_client_wrapper);
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

