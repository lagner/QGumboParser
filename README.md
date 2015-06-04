## Introduction
If you need to parse HTML page in Qt application it can be a problem. Qt doesn't have a HTML parser. You can use [gumbo-parser]("https://github.com/google/gumbo-parser") developed by google but it was written in pure C and doesn't provide Qt-like interface. Therefore not so comfortable to work with. This small library solves the issue. 

## Quick Start
The easist way to use QGumboParser is add it to your project as git submodule.  
To add the library use the following steps:
- Create Subdirs Project.
- Add application subproject. "Qt Console Application" for example
- Open project folder and create *libs* directory
- Run `git submodule add git@github.com:lagner/QGumboParser.git libs/QGumboParser` in terminal.
- Run `git submodule update --init --recursive`
- Add `SUBDIRS += libs/QGumboParser/QGumboParser` into the root project. QGumboParser have to appear in your project tree
- Right click to application project that need Html parser and hit Add library -> Internal library -> select QGumboParser in combobox. Click finish.

The library is ready to use.  
Please pay attention the library requires c++11 support(just add *"CONFIG += c++11"* into your .pro file).
## Example
```
#include <QCoreApplication>
#include <QDebug>
#include <qgumbodocument.h>
#include <qgumbonode.h>


const char* HTML_PAGE = R"~("
<!DOCTYPE html>
<html>
  <head>
    <title>Title text</title>
    <meta content="">
    <style></style>
  </head>
  <body>
    <h3>First header</h3>
    <p>text text text</p>
    <div class="content">
        <h3>Nested header</h3>
    </div>
  </body>
</html>
")~";

int main()
{
    auto doc = QGumboDocument::parse(HTML_PAGE);
    auto root = doc.rootNode();
    auto nodes = root.getElementsByTagName(HtmlTag::TITLE);
    Q_ASSERT(nodes.size() == 1);

    auto title = nodes.front();
    qDebug() << "title is: " << title.innerText();

    nodes = root.getElementsByTagName(HtmlTag::H3);
    for (const auto& node: nodes) {
        qDebug() << "h3: " << node.innerText();
    }
    return 0;
}

```

## License
MIT License. See LICENSE file