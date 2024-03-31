[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MS5611

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ms5611/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der MS5611 ist eine neue Generation hochauflösender Höhenmessersensoren von MEAS Switzerland mit SPI- und I2C-Bus-Schnittstelle. Dieser Luftdrucksensor ist für Höhenmesser und Variometer mit einer Höhenauflösung von 10 cm optimiert. Das Sensormodul umfasst einen Drucksensor mit hoher Linearität und einen 24-Bit-ΔΣ-ADC mit extrem geringem Stromverbrauch und internen, werkseitig kalibrierten Koeffizienten. Es bietet einen präzisen digitalen 24-Bit-Druck- und Temperaturwert und verschiedene Betriebsmodi, die es dem Benutzer ermöglichen, die Umwandlungsgeschwindigkeit und den Stromverbrauch zu optimieren. Ein hochauflösender Temperaturausgang ermöglicht die Implementierung einer Höhenmesser-/Thermometerfunktion ohne zusätzlichen Sensor. Der MS5611 kann an praktisch jeden Mikrocontroller angeschlossen werden. Das Kommunikationsprotokoll ist einfach und erfordert keine Programmierung interner Register im Gerät. Geringe Abmessungen von nur 5,0 mm x 3,0 mm und eine Höhe von nur 1,0 mm ermöglichen die Integration in mobile Geräte. Diese neue Sensormodulgeneration basiert auf führender MEMS-Technologie und den neuesten Vorteilen der bewährten Erfahrung und des Know-hows von MEAS Switzerland in der Großserienfertigung von Höhenmessermodulen, die seit über einem Jahrzehnt weit verbreitet sind. Das verwendete Sensorprinzip führt zu einer sehr geringen Hysterese und einer hohen Stabilität sowohl des Druck- als auch des Temperatursignals.

LibDriver MS5611 ist ein voll funktionsfähiger Treiber von MS5611, der von LibDriver gestartet wurde. Er bietet Druck- und Temperaturmessungen und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MS5611-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MS5611 IIC, SPI.

/test enthält den Testcode des LibDriver MS5611-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MS5611-Beispielcode.

/doc enthält das LibDriver MS5611-Offlinedokument.

/Datenblatt enthält MS5611-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_ms5611_basic.h"

uint8_t res;
uint32_t i;

/* basic init */
res = ms5611_basic_init(MS5611_INTERFACE_IIC, MS5611_ADDRESS_CSB_0);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    float temperature_c;
    float pressure_mbar;

    /* read data */
    res = ms5611_basic_read(&temperature_c, &pressure_mbar);
    if (res != 0)
    {
        (void)ms5611_basic_deinit();

        return 1;
    }

    /* output */
    ms5611_interface_debug_print("ms5611: %d/%d.\n", i + 1, 3);
    ms5611_interface_debug_print("ms5611: temperature is %0.2fC.\n", temperature_c);
    ms5611_interface_debug_print("ms5611: pressure is %0.2fmbar.\n", pressure_mbar);

    /* delay 1000ms */
    ms5611_interface_delay_ms(1000);
    
    ...
}

...
    
/* basic deinit */
(void)ms5611_basic_deinit();

return 0;
```
### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ms5611/index.html](https://www.libdriver.com/docs/ms5611/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.