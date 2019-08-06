// ************************************
// Copyrights by Jin Fagang
// 2/15/19-15-17
// app_license
// jinfagang19@gmail.com
// Apache License
// ************************************

//
// Created by jintain on 2/15/19.
//

#ifdef USE_CURL

#include "../include/app_license.h"

thor::security::LicenseChecker::LicenseChecker()
{
  _url = "http://loliloli.pro/api/v1/unique_license";
}

thor::security::CheckResult thor::security::LicenseChecker::checkLicense(string license)
{
  string api_url;
  api_url = (_url + "?license=" + license);

  CURL *_curl = curl_easy_init();
  if (!_curl)
  {
    // error to
    LOG(ERROR) << "can not launch curl instance, is curl installed? or network connected?\n";
    exit(1);
  }
  LOG(INFO) << "satellite initialized.\n";

  CURLcode res;
  long httpCode(0);

  std::string readBuffer;

  curl_easy_setopt(_curl, CURLOPT_URL, api_url.c_str());
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
  curl_easy_setopt(_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
  curl_easy_setopt(_curl, CURLOPT_TIMEOUT, 10);

  res = curl_easy_perform(_curl);
  curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &httpCode);
  LOG(INFO) << "checking done.";

  CheckResult checkResult;
  if (httpCode != CURLE_HTTP_RETURNED_ERROR)
  {
    json j_res = json::parse(readBuffer);

    if (j_res["status"] == "success")
    {
      checkResult.ok = true;
      checkResult.msg = "license ok";
    }
    else
    {
      checkResult.ok = false;
      if (j_res.find("message") != j_res.end())
      {
        // there is an entry with key "foo"
        checkResult.msg = j_res["message"];
      }
      else
      {
        checkResult.msg = j_res["data"];
      }
    }
  }
  else
  {
    LOG(ERROR) << "connect cloud error, either offline or cloud down.";
    checkResult.ok = false;
    checkResult.msg = "network is not ok.";
  }
  curl_easy_cleanup(_curl);
  return checkResult;
}

size_t thor::security::LicenseChecker::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
#endif