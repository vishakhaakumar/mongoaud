#ifndef MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H
#define MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/RecommenderService.h"
#include "../../gen-cpp/WeatherService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class RecommenderServiceHandler : public RecommenderServiceIf {
 public:
  RecommenderServiceHandler(
		  ClientPool<ThriftClient<WeatherServiceClient>> *) ;
  ~RecommenderServiceHandler() override=default;

  void GetRecommendations(std::vector<Movie>& _return, const int64_t user) override;
 private:
  ClientPool<ThriftClient<WeatherServiceClient>> *_weather_client_pool;
};

// Constructor
RecommenderServiceHandler::RecommenderServiceHandler(
		ClientPool<ThriftClient<WeatherServiceClient>> *weather_client_pool) {

     // Storing the clientpool
     _weather_client_pool = weather_client_pool;
}

// Remote Procedure "GetRecommendations"
void RecommenderServiceHandler::GetRecommendations(std::vector<Movie>& _return, const int64_t user){
     printf("GetRecommendations\n");

     Movie movie_1;
     Movie movie_2;

     movie_1.movie_id = 123L;
     movie_1.title = "Waking Ned Devine";

     movie_2.movie_id = 456L;
     movie_2.title = "Pitch Perfect";

     _return.push_back(movie_1);
     _return.push_back(movie_2);
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

