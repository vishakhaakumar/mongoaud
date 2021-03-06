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

  void GetRecommendations(std::string& _return, const int64_t user) override;
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
void RecommenderServiceHandler::GetRecommendations(std::string& _return, const int64_t user){
     printf("GetRecommendations\n");

     _return = "Audrey";
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_RECOMMENDERHANDLER_H

