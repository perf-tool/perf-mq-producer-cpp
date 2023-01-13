/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <sstream>
#include <pulsar/Client.h>
#include "env_util.cpp"
#include "random_util.cpp"

using namespace std;
using namespace pulsar;

static string pulsarHost = EnvUtil::getEnv("PULSAR_HOST", "localhost");
static int pulsarPort = EnvUtil::getEnvInt("PULSAR_PORT", 6650);
static bool pulsarTlsEnable = EnvUtil::getEnvBool("PULSAR_TLS_ENABLE", false);
static bool pulsarTlsAllowInsecureConnection = EnvUtil::getEnvBool("PULSAR_TLS_ALLOW_INSECURE_CONNECTION", false);
static string pulsarAuthToken = EnvUtil::getEnv("PULSAR_AUTH_TOKEN", "");

class Pulsar {
public:
    [[noreturn]] static void start() {
        ClientConfiguration configuration = ClientConfiguration();
        if (!pulsarAuthToken.empty()) {
            configuration.setAuth(AuthToken::createWithToken(pulsarAuthToken));
        }
        if (pulsarTlsEnable) {
            configuration.setTlsAllowInsecureConnection(pulsarTlsAllowInsecureConnection);
        }
        Client client(getPulsarUrl(), configuration);
        Producer producer;
        Result result = client.createProducer(getenv("PULSAR_TOPIC"), producer);
        if (result != ResultOk) {
            std::cout << "create producer failed" << std::endl;
        } else {
            std::cout << "create producer success" << std::endl;
        }
        for (;;) {
            const Message &msg = MessageBuilder().setContent(RandomUtil::randomStr(messageSize)).build();
            Result res = producer.send(msg);
            if (res != ResultOk) {
                std::cout << "send message failed" << std::endl;
            } else {
                std::cout << "send message success" << std::endl;
            }
        }
    }

private:
    static string getPulsarUrl() {
        std::ostringstream stringStream;
        if (pulsarTlsEnable) {
            stringStream << "pulsar+ssl://";
        } else {
            stringStream << "pulsar://";
        }
        stringStream << pulsarHost << ":" << pulsarPort;
        return stringStream.str();
    }
};
