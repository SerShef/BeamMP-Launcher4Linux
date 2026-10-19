/*
 Copyright (C) 2024 BeamMP Ltd., BeamMP team and contributors.
 Licensed under AGPL-3.0 (or later), see <https://www.gnu.org/licenses/>.
 SPDX-License-Identifier: AGPL-3.0-or-later
*/


#include "Http.h"
#include <Logger.h>
#include <Network/network.hpp>
#include <Startup.h>
#include <Utils.h>
#include <cmath>
#include <curl/curl.h>
#include <curl/easy.h>
#include <filesystem>
#include <fstream>
#include <httplib.h>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>


static size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* Result = reinterpret_cast<std::string*>(userp);
    std::string NewContents(reinterpret_cast<char*>(contents), size * nmemb);
    *Result += NewContents;
    return size * nmemb;
}

bool HTTP::isDownload = false;
std::string HTTP::Get(const std::string& IP) {
    std::string Ret;
    static thread_local CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        char errbuf[CURL_ERROR_SIZE];
        curl_easy_setopt(curl, CURLOPT_URL, IP.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&Ret);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 120); // seconds
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        errbuf[0] = 0;
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            error("GET to " + IP + " failed: " + std::string(curl_easy_strerror(res)));
            error("Curl error: " + std::string(errbuf));
            return "";
        }
    } else {
        error("Curl easy init failed");
        return "";
    }
    return Ret;
}

std::string HTTP::Post(const std::string& IP, const std::string& Fields) {
    std::string Ret;
    static thread_local CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        char errbuf[CURL_ERROR_SIZE];
        curl_easy_setopt(curl, CURLOPT_URL, IP.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&Ret);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Fields.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Fields.size());
        struct curl_slist* list = nullptr;
        list = curl_slist_append(list, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 120); // seconds
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        errbuf[0] = 0;
        res = curl_easy_perform(curl);
        curl_slist_free_all(list);
        if (res != CURLE_OK) {
            error("POST to " + IP + " failed: " + std::string(curl_easy_strerror(res)));
            error("Curl error: " + std::string(errbuf));
            return "";
        }
    } else {
        error("Curl easy init failed");
        return "";
    }
    return Ret;
}

bool HTTP::Download(const std::string& IP, const beammp_fs_string& Path, const std::string& Hash) {
    static std::mutex Lock;
    std::scoped_lock Guard(Lock);

    info("Downloading an update (this may take a while)");
    std::string Ret = Get(IP);

    if (Ret.empty()) {
        error("Download failed");
        return false;
    }

    std::string RetHash = Utils::GetSha256HashReallyFast(Ret, Path);

    debug("Return hash: " + RetHash);
    debug("Expected hash: " + Hash);

    if (RetHash != Hash) {
        error("Downloaded file hash does not match expected hash");
        return false;
    }

    std::ofstream File(Path, std::ios::binary);
    if (File.is_open()) {
        File << Ret;
        File.close();
        info("Download Complete!");
    } else {
        error(beammp_wide("Failed to open file directory: ") + Path);
        return false;
    }

    return true;
}


void HTTP::StartProxy() {}
