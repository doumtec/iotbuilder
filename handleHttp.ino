String webTail="</body> </html>";

String webMenu="";
String webHtml="";

//***********************************************************************
char Head[] PROGMEM = R"=====(
<!DOCTYPE html>
<html class='loading' lang='kr' data-textdirection='ltr'>
<head>
<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>
<title>GOODMEN</title>
<link rel='apple-touch-icon' href='http://goodmen.co.kr/assets/icon/apple-icon-120.png'>
<link href='http://goodmen.co.kr/assets/icon/favicon.png' rel='icon'>
<link rel='stylesheet' type='text/css' href='http://goodmen.co.kr/firmware/firmware_all.css'>    
</head>
)=====";

char ScriptRoot[] PROGMEM = R"=====(
<script src='http://goodmen.co.kr/firmware/ScriptRoot.js'></script>
</head>
<body onload="javascript:init()" class='horizontal-layout horizontal-menu navbar-floating footer-static bg-bgray-tint' data-open='hover' data-menu='horizontal-menu' data-col=''>
)=====";


char ScriptHead[] PROGMEM = R"=====(
<script src='http://goodmen.co.kr/firmware/ScriptHead.js'></script>
</head>
<body onload="javascript:init()" class='horizontal-layout horizontal-menu navbar-floating footer-static bg-bgray-tint' data-open='hover' data-menu='horizontal-menu' data-col=''>
)=====";

char Body[] PROGMEM = R"=====(
<div class='content app-firmware-content'>
<div class='content-overlay'></div>
<div class='content-wrapper'>
<div class='content-header row'>
</div>
<div class='content-body'>
<div class='card'>
<div class='card-header text-center'>
<h4 class='card-title w-100 text-center'>로컬 서버</h4>
</div>
<div class='card-body mh-50vh'>
<div class='now-data d-flex-box'>
<h5 class='font-medium-1'>온도</h5>
<div class='font-large-1 text-tertiary'><span id='temp'>0</span><span class='font-small-2'>℃</span></div>
</div>
<div class='now-data d-flex-box'>
<h5 class='font-medium-1'>습도</h5>
<div class='font-large-1 text-info'><span id='humi'>0</span><span class='font-small-2'>%</span></div>
</div>
</div>
</div>
</div>
</div>
</div>
)=====";

char BodyControl[] PROGMEM = R"=====(
  <br>출력<br>

)=====";

char Menu[] PROGMEM = R"=====(
<script src='http://goodmen.co.kr/firmware/firmware_head.js'></script>
)=====";

char Download[] PROGMEM = R"=====(
<div class='content app-firmware-content'>
<div class='content-overlay'></div>
<div class='content-wrapper'>
<div class='content-header row'>
</div>
<div class='content-body'>
<div class='card'>
<div class='card-header text-center'>
<h4 class='card-title w-100 text-center'>다운로드</h4>
</div>
<div class='card-body mh-50vh'>
<div>
다운로드 추가된 기기를 다운받으려면 펌웨어 업그레이드를 하세요.
<div class='mt-50'><button class='btn btn-outline-primary' onclick="sendAct('#'+'{\'act\':1}');">최신 펌웨어 업그레이드</button>
</div>
</div>
</div>
</div>
</div>
</div>
)=====";

char Manual[] PROGMEM = R"=====(
<div class='content app-firmware-content'>
<div class='content-overlay'></div>
<div class='content-wrapper'>
<div class='content-header row'>
</div>
<div class='content-body'>
<div class='card'>
<div class='card-header text-center'>
<h4 class='card-title w-100 text-center'>메뉴얼</h4>
</div>
<div class='card-body mh-50vh'>
<ul class='pl-1'>
<li>1. 메뉴얼 내용 입력</li>
<li>1. 메뉴얼 내용 입력</li>
</div>
</div>
</div>
</div>
</div>
</div>
)=====";

char Tail[] PROGMEM = R"=====(
</body></html>
)=====";

void handleRoot() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(ScriptRoot);
  s+=FPSTR(Menu);
  s+=FPSTR(Body);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}

void handleControl() {
  String sOut="";
  sOut=FPSTR(Head);
  sOut+=FPSTR(ScriptHead);
  sOut+=FPSTR(Menu);
  sOut+=FPSTR(BodyControl);
  sOut+=FPSTR(Tail);
  server.send(200, "text/html", sOut);
}

void handleOn() {
  int actIn=0;
  actIn=server.arg("act").toInt();

  if(actIn==2) {
    server.arg("email").toCharArray(email, sizeof(email) - 1);
    Serial.println(email);
    saveConfig();
  }
  GoHome();  
}

void GoHome() {
  String s;
  s="<meta http-equiv='refresh' content=\"0;url='http://"+ipAct+"/'\">";
  server.send(200, "text/html", s);
}

void GoHomeWifi() {
  String s,ipS;
  IPAddress ip;
  if(bootMode==1)
    ip = apIP;
  else
    ip = WiFi.localIP();
  
  ipS=String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
    s="<meta http-equiv='refresh' content=\"0;url='http://"+ipS+"/wifi'\">";
  server.send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//와이파이 강도측정
String sensWifi(int in) {
  if(in < -80)
    return "9783";
  else if(in < -70)
    return "9779";
  else if(in < -50)
    return "9777";
  else 
    return "9776";
}

void handleWifi() {
  String s=""; 
  String s1= String(ssid);
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  delay(100);

  s="<div class='content app-firmware-content'>";
  s+="<div class='content-overlay'></div>";
  s+="<div class='content-wrapper'>";
  s+="<div class='content-header row'></div>";
  s+="<div class='content-body'>";
  s+="<div class='card'>";
  s+="<div class='card-header text-center'><h4 class='card-title w-100 text-center'>WiFi</h4></div>";
  s+="<div class='card-body mh-50vh'>";
  s+="<div class='form-group'>";
  s+="<label for='wifi' class='form-label'>현재 연결된 WiFi</label>";
  Serial.println("헤더-001");

  if (n > 0) {
    //s+="검색된 와이파이";
    s+="<select id='ssid' name='ssid' onclick=\"sendOption();\" class='form-control form-control-lg' >";
    for (int i = 0; i < n; i++) {
      s+="<option value='"+WiFi.SSID(i)+"'>";
      s+=WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"&emsp;&#128274")+"&emsp; &#"+ sensWifi(WiFi.RSSI(i));
      s+="</option>";
     Serial.print("와이파이-");
     Serial.println(i);
    }
    s+="</select>";
  } 
  else 
    s+="<br>No WLAN found";

  s+="<div class='mb-2'><a href='/wifi'><button class='btn btn-outline-primary w-100'><i data-feather='refresh-ccw'></i> WiFi 다시 검색</button></a></div>";
  s+="<form method='POST' action='/wifisave'>";
  s+="<div class='form-group border-top-mute pt-1'><label for='name' class='form-label'>WiFi 이름 </label><input type='text' class='form-control form-control-lg' id='ssidSelected' value='"+(String)ssid+"' name='n'/></div>";
  s+="<div class='form-group'><label for='password' class='form-label'>비밀번호 </label><input type='password' class='form-control form-control-lg'  value='"+(String)password+"' name='p' /></div>";
  s+="<div><button type='submit' class='btn btn-primary w-100'>저장</button></div>";
  s+="</form>";

  s+="</div></div></div></div></div>";
  String sOut;
  sOut=FPSTR(Head);
  sOut+=FPSTR(ScriptHead);
  sOut+=FPSTR(Menu);
  sOut+=s;
  sOut+=FPSTR(Tail);
  server.send(200, "text/html", sOut);
}
/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  displayOled(4);
  GoHome();
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveConfig();
  //connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
  Serial.println(WiFi.waitForConnectResult());
  Serial.println(ssid);
  Serial.println(password);
  Serial.println("Reset");
  ESP.reset();
  delay(2000);
}

void handleScan() {
  String s;
  s="{\"mac\":\""+sMac+"\",\"ip\":\""+WiFi.localIP().toString()+"\",\"type\":"+type+",\"ec\":"+ec+",\"ph\":"+ph+",\"temp\":"+temp+"}";
  server.send(200, "text/html", s);
}

void handleConfig() {
  String s;
  s ="<div class='content app-firmware-content'>";
  s+="<div class='content-overlay'></div>";
  s+="<div class='content-wrapper'>";
  s+="<div class='content-header row'></div>";
  s+="<div class='content-body'>";
  s+="<div class='card'>";
  s+="<div class='card-header text-center'><h4 class='card-title w-100 text-center'>환경설정</h4></div>";
  s+="<div class='card-body mh-50vh'>";
  s+="<div>Email을 등록하시고 goodmen.co.kr 에서 회원가입을 하셔야 이용이 가능합니다.</div>";
  s+="<form action='/on'>";
  s+="<input type='hidden' name='act' value='2'>";
  s+="<div class='form-group pt-1 mt-2 border-top-mute'><label for='mqtt' class='form-label'>E-Mail </label><input type='text' class='form-control form-control-lg' name='email' id='email' value='"+(String)email+"' /></div>";
  s+="<div><button class='btn btn-primary w-100' type='submit'>저장</button></div>";
  s+="</form>";
  s+="</div>";
  s+="</div>";
  s+="</div>";
  s+="</div>";
  s+="</div>";
  
  String sOut;
  sOut=FPSTR(Head);
  sOut+=FPSTR(ScriptHead);
  sOut+=FPSTR(Menu);
  sOut+=s;
  sOut+=FPSTR(Tail);
  server.send(200, "text/html", sOut);
}

void handleDownload() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(ScriptHead);
  s+=FPSTR(Menu);
  s+=FPSTR(Download);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}

void handleManual() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(ScriptHead);
  s+=FPSTR(Menu);
  s+=FPSTR(Manual);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}
