// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
suite("test_ip_functions", "arrow_flight_sql") {
    sql "set batch_size = 4096;"

    qt_sql "SELECT ipv4numtostring(-1);"
    qt_sql "SELECT ipv4numtostring(2130706433);"
    qt_sql "SELECT ipv4numtostring(4294967298);"
    qt_sql "SELECT inet_ntoa(3232235521);"

    qt_sql "SELECT ipv4stringtonum('127.0.0.1');"
    qt_sql "SELECT ipv4stringtonumornull('');"
    qt_sql "SELECT ipv4stringtonumordefault('');"
    qt_sql "SELECT inet_aton('192.168.0.1');"

    qt_sql "SELECT ipv6numtostring(unhex('0A0005091'));"
    qt_sql "SELECT ipv6numtostring(unhex('2A0206B8000000000000000000000011'));"
    qt_sql "SELECT ipv6numtostring(unhex('FDFE0000000000005A55CAFFFEFA9089'));"
    qt_sql "SELECT ipv6numtostring(unhex(''));"
    qt_sql "SELECT ipv6numtostring(unhex('KK'));"
    qt_sql "SELECT ipv6numtostring(unhex('0A000509'));"
    qt_sql "SELECT ipv6numtostring(unhex('abcd123456'));"
    qt_sql "SELECT ipv6numtostring(unhex('ffffffffffffffffffffffffffffffffffffffffffffffffffffff'));"
    qt_sql "SELECT inet6_ntoa(unhex('0A0005091'));"
    qt_sql "SELECT inet6_ntoa(unhex('2A0206B8000000000000000000000011'));"
    qt_sql "SELECT inet6_ntoa(unhex(NULL));"
    qt_sql "SELECT inet6_ntoa(unhex('00000000000000000000000000000000'));"
    qt_sql "SELECT inet6_ntoa(unhex('0000000000000000000000000000'));"
    qt_sql "SELECT inet6_ntoa(unhex('000'));"
    qt_sql "SELECT inet6_ntoa(unhex('aaaaaaaaFFFFFFFFFFFFFFFFaaaaaaaa'));"
    qt_sql "SELECT inet6_ntoa(unhex('aaaa@#'));"
    qt_sql "SELECT inet6_ntoa(unhex('\0'));"
    qt_sql "SELECT inet6_ntoa(unhex('00000000000000000000FFFF7F000001'));"
}