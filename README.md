# Qt app for getting currency quotes Bank XML service.

This application gets currency quotes from Central Bank of Russian Fedderation site.
URLs like "http://www.cbr.ru/scripts/XML_daily.asp?" are processed.
New: it now stores quotes in local database SQLite.
I18n is possible.

Copyright 2017 Denis Trofimov <silaradost@yandex.ru>

Uses code of tutorial projects XmlDOMRead, DownloaderGui by Max Schlee from the
book "Qt 5.3 Professional programming with C++".

With all due respect thanks to Max Schlee as a great teacher.

Проект дал мне возможный работодатель как тест Qt C++. Поскольку мне интересно овладеть Qt, взялся с интересом.

Задача в том, чтобы достать котировки валют с сайта ЦБ РФ, отобразить пользователю. Записать котировки в локальную БД SQLite3, и в дальнейшем при повторе ввода загружать котировки из БД и идти на сайт только за новыми данными.

Интересно, что в этом учебном проекте я уже сделал загрузку по http, парсинг и составление URL, парсинг XML документа через DOM модель (попробовал и stream). Проектирование GUI с нуля, формы, кнопки, обработка - валидация ввода.
Сейчас дописываю чтение - запись - выборки SQL запросами из БД SQLite3. Основные функции реализованы. На удивление легко построилась реляционная модель БД, правда не силами Qt, а мощью SQL92. Очень помогло, что на сайте http://www.sql-ex.ru/ решил первые 30 задач на SQL.

Для отладки и для интереса добавляю модель БД/представление, чтобы посмотреть, что творится в БД. Скоро новый релиз. https://github.com/denis-trofimov/qt-currency-quotes-CBRF/releases

Думал о локализации, предварительно проект готов к переводу. Посмотрел, как это делается на тестовом проекте в Qt Linguist, смогу перевести когда функционал БД завершу.

// ======================================================================
//                   This file is a part of the book
//             "Qt 5.3 Professional programming with C++"
// ======================================================================
//  Copyright (c) 2014 by Max Schlee
//
//  Email : Max.Schlee@neonway.com
//  Blog  : http://www.maxschlee.com
//
//  Social Networks
//  ---------------
//  FaceBook : http://www.facebook.com/mschlee
//  Twitter  : http://twitter.com/Max_Schlee
//  2Look.me : http://2look.me/NW100003
//  Xing     : http://www.xing.com/profile/Max_Schlee
//  vk.com   : https://vk.com/max.schlee
// ======================================================================
