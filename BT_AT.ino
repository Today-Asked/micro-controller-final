#include <SoftwareSerial.h>   // 引用程式庫

// 定義連接藍牙模組的序列埠
SoftwareSerial BT(3, 2); // 接收腳, 傳送腳
char val;  // 儲存接收資料的變數
int state = 0;
String receivedData = "";  // 儲存接收的資料

void setup() {
  Serial.begin(9600);   // 與電腦序列埠連線
  Serial.println("BT is ready!");

  // 設定藍牙模組的連線速率
  BT.begin(9600);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  // 設定port 3-9為輸入腳位
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

  // 藍牙連線初始化過程
  sendATCommand("AT");  // 發送 AT 指令
  if (waitForResponse("OK")) {  // 等待 OK 回應
    receivedData = "";  // 清空接收資料
    sendATCommand("AT+INQ");  // 發送 AT+INQ 指令
    if (waitForResponse("+INQE")) {  // 等待 +INQE 回應
      receivedData = "";  // 清空接收資料
      sendATCommand("AT+CONN1");  // 發送 AT+CONN1 指令來連接
    }
  }
}

void loop() {
  if (state == 1) {
    // 判斷port 3-9的狀態，並根據狀態發送不同指令到藍牙模組
    if (digitalRead(4) == 1) {
      BT.println("LRN");
      Serial.print("LRN\r\n");
    }
    else if (digitalRead(4) == 0) {
      BT.println("LRF");
      Serial.print("LRF\r\n");
    }
    if (digitalRead(7) == 1) {
      BT.println("LLN");
      Serial.print("LLN\r\n");
    }
    else if (digitalRead(7) == 0) {
      BT.println("LLF");
      Serial.print("LLF\r\n");
    }
    if (digitalRead(8) == 1) {
      BT.println("BN");
      Serial.print("BN\r\n");
    }
    else if (digitalRead(8) == 0) {
      BT.println("BF");
      Serial.print("BF\r\n");
    }

    delay(500);
  } else if (state == 0) {
    // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
    if (BT.available()) {
      val = BT.read();
      receivedData += val;  // 將接收到的資料拼接到receivedData字串中

      // 當接收到完整的"+Connected"時，將state設為1
      if (receivedData.indexOf("+Connected") >= 0) {
        state = 1;
        Serial.println("State changed to 1.");
      }

      Serial.print(val);  // 顯示接收到的資料
    }

    // 若收到「序列埠監控視窗」的資料，則送到藍牙模組
    if (Serial.available()) {
      val = Serial.read();
      BT.print(val);
    }
  }
}

// 發送 AT 指令並等待回應
void sendATCommand(String command) {
  BT.println(command);  // 發送指令到藍牙模組
  Serial.println("Sending: " + command);  // 顯示發送的指令
  receivedData = "";  // 清空接收資料
}

// 使用 while 迴圈等待回應
bool waitForResponse(String expectedResponse) {
  unsigned long startMillis = millis();  // 記錄等待開始時間
  unsigned long timeout = 10000;  // 設定超時時間（例如 5 秒）

  while (millis() - startMillis < timeout) {
    if (BT.available()) {
      char c = BT.read();
      receivedData += c;  // 將接收到的資料拼接到receivedData字串中

      // 檢查是否收到預期的回應
      if (receivedData.indexOf(expectedResponse) >= 0) {
        Serial.println("Received expected response: " + expectedResponse);
        return true;  // 返回 true，表示已收到預期回應
      }
    }
  }

  // 如果超過超時時間仍未收到預期回應
  Serial.println("Timeout waiting for response: " + expectedResponse);
  return false;  // 返回 false，表示超時未收到回應
}
