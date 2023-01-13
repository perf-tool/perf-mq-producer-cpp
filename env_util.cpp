/*
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

#include <iostream>

using namespace std;

class EnvUtil {
public:
    static string getEnv(const char *name, string defaultValue) {
        char *value = getenv(name);
        if (value == nullptr) {
            return defaultValue;
        }
        return {value};
    }

    static int getEnvInt(const char *name, int defaultValue) {
        char *aux = getenv(name);
        if (aux == nullptr) {
            return defaultValue;
        }
        return atoi(aux);
    }

    static int getEnvBool(const char *name, bool defaultValue) {
        char *aux = getenv(name);
        if (aux == nullptr) {
            return defaultValue;
        }
        return strCmpIgnoreCase(aux, "true");
    }

    static char tolower(unsigned char ch) {
        if (ch >= 'A' && ch <= 'Z')
            ch = 'a' + (ch - 'A');
        return ch;
    }

    static bool strCmpIgnoreCase(const char *s1, const char *s2) {
        const auto *us1 = (const u_char *)s1,
                *us2 = (const u_char *)s2;

        while (tolower(*us1) == tolower(*us2++))
            if (*us1++ == '\0')
                return true;
        return false;
    }
};
