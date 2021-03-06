#ifndef MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H
#define MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/RecommenderService.h"
#include "../../gen-cpp/MovieInfoService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class RecommenderServiceHandler : public RecommenderServiceIf {
 public:
  RecommenderServiceHandler(
		  ClientPool<ThriftClient<MovieInfoServiceClient>> *) ;
  ~RecommenderServiceHandler() override=default;

  void GetRecommendations(std::vector<std::string>& _return, const int64_t user) override;
 private:
  ClientPool<ThriftClient<MovieInfoServiceClient>> *_movie_info_client_pool;
};

// Constructor
RecommenderServiceHandler::RecommenderServiceHandler(
		ClientPool<ThriftClient<MovieInfoServiceClient>> *movie_info_client_pool) {

     // Storing the clientpool
     _movie_info_client_pool = movie_info_client_pool;
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
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

