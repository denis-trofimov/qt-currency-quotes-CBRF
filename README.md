# Qt app for getting currency quotes from Central Bank of Russian Fedderation

This application gets currency quotes from Central Bank of Russian Fedderation site.
URLs like "http://www.cbr.ru/scripts/XML_daily.asp?" are processed.
* It now stores quotes in local database SQLite.
* I18n: system locale dependent UI, english default, russian secondary.

* Have Debian 8 compatible binary, no package yet.

Debian 8 32 screenshot.
![2017-02-13 01 01 53](https://cloud.githubusercontent.com/assets/24657082/22866479/bc88ba22-f190-11e6-9246-eba55d4a248f.png) | ![2017-02-13 01 10 17_0 2-beta3_en_ru](https://cloud.githubusercontent.com/assets/24657082/22866543/b777fa7e-f191-11e6-963c-f9eb2f06bead.png)

* Have win32 Windows binary 7z packed. Ready to deploy, unpack with 7z and run.

Windows 7 32 screenshot.
![windows 7 qt-currency-quotes-cbrf](https://cloud.githubusercontent.com/assets/24657082/22822539/edddaa08-ef90-11e6-8c84-373d12d34467.png)

TODO: static linking
TODO: Foreign Currency Market Reference

With all due respect thanks to Max Schlee <Max.Schlee@neonway.com> as a great teacher.
My project uses code of tutorial projects XmlDOMRead, DownloaderGui from the
book "Qt 5.3 Professional programming with C++" by Max Schlee .

####### Below old text from one letter.

Проект дал мне возможный работодатель как тест Qt C++. Поскольку мне интересно овладеть Qt, взялся с интересом.

Задача в том, чтобы достать котировки валют с сайта ЦБ РФ, отобразить пользователю. Записать котировки в локальную БД SQLite3, и в дальнейшем при повторе ввода загружать котировки из БД, и идти на сайт только за новыми данными.

Интересно, что в этом учебном проекте я уже сделал загрузку по http, парсинг и составление URL, парсинг XML документа через DOM модель (попробовал и stream). Проектирование GUI с нуля, формы, кнопки, обработка - валидация ввода.
Сейчас дописываю чтение - запись - выборки SQL запросами из БД SQLite3. Основные функции реализованы. На удивление легко построилась реляционная модель БД, правда не силами Qt, а мощью SQL92. Очень помогло, что на сайте http://www.sql-ex.ru/ решил первые 30 задач на SQL.

Для отладки и для интереса добавляю модель БД/представление, чтобы посмотреть, что творится в БД. Скоро новый релиз. https://github.com/denis-trofimov/qt-currency-quotes-CBRF/releases

Думал о локализации, предварительно проект готов к переводу. Посмотрел, как это делается на тестовом проекте в Qt Linguist, смогу перевести когда функционал БД завершу.
