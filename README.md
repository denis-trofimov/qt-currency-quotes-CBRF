# Qt app for getting currency quotes from Central Bank of Russian Fedderation

This application gets currency quotes from Central Bank of Russian Fedderation site.
URLs like "http://www.cbr.ru/scripts/XML_daily.asp?" are processed.
It now stores quotes in local database SQLite.
Have I18n files, English translation default, English source code.
Have win32 Windows binary ready to deploy 7z packed.

![windows 7 qt-currency-quotes-cbrf](https://cloud.githubusercontent.com/assets/24657082/22822539/edddaa08-ef90-11e6-8c84-373d12d34467.png)

Have Debian 8 compatible binary, no package yet.

TODO: static linking
TODO: Foreign Currency Market Reference

Copyright 2017 Denis Trofimov <silaradost@yandex.ru>

Main widget Debian 8

![2017-02-07 22 04 42](https://cloud.githubusercontent.com/assets/24657082/22707435/36c6ad70-ed8c-11e6-929a-38adca182511.png)

Tables for inspecting model/view of SQLite.

![2017-02-07 22 05 09](https://cloud.githubusercontent.com/assets/24657082/22707436/36c754aa-ed8c-11e6-828d-46c199ed6de7.png)

![2017-02-07 22 05 34](https://cloud.githubusercontent.com/assets/24657082/22707437/36c9dad6-ed8c-11e6-8c5f-feaac7dbcacb.png)

Uses code of tutorial projects XmlDOMRead, DownloaderGui from the
book "Qt 5.3 Professional programming with C++" by Max Schlee <Max.Schlee@neonway.com> .
With all due respect thanks to Max Schlee as a great teacher.

Проект дал мне возможный работодатель как тест Qt C++. Поскольку мне интересно овладеть Qt, взялся с интересом.

Задача в том, чтобы достать котировки валют с сайта ЦБ РФ, отобразить пользователю. Записать котировки в локальную БД SQLite3, и в дальнейшем при повторе ввода загружать котировки из БД, и идти на сайт только за новыми данными.

Интересно, что в этом учебном проекте я уже сделал загрузку по http, парсинг и составление URL, парсинг XML документа через DOM модель (попробовал и stream). Проектирование GUI с нуля, формы, кнопки, обработка - валидация ввода.
Сейчас дописываю чтение - запись - выборки SQL запросами из БД SQLite3. Основные функции реализованы. На удивление легко построилась реляционная модель БД, правда не силами Qt, а мощью SQL92. Очень помогло, что на сайте http://www.sql-ex.ru/ решил первые 30 задач на SQL.

Для отладки и для интереса добавляю модель БД/представление, чтобы посмотреть, что творится в БД. Скоро новый релиз. https://github.com/denis-trofimov/qt-currency-quotes-CBRF/releases

Думал о локализации, предварительно проект готов к переводу. Посмотрел, как это делается на тестовом проекте в Qt Linguist, смогу перевести когда функционал БД завершу.
