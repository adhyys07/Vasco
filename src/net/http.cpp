#include "net/http.h"

namespace {
const char* kBuiltInPage = R"HTML(<!DOCTYPE html>
<html>
<head><title>minibrowser test page</title></head>
<body>
  <h1>It renders.</h1>
  <p>This page is compiled into the binary, so layout and painting can be
     debugged before the network layer exists. Here is <b>bold</b> text,
     some <i>italic</i> text, and an entity: &copy; 2026 &amp; counting.</p>
  <h2>A list</h2>
  <ul>
    <li>Block layout stacks boxes vertically.</li>
    <li>Inline layout fills lines and wraps at the right edge.</li>
    <li>A long item so you can watch the line breaking actually work when you
        drag the window narrower and the text reflows to fit the new width.</li>
  </ul>
  <h2>Preformatted</h2>
  <pre>  whitespace   is preserved here
  and newlines are too</pre>
  <p>A <a href="/hypertext/WWW/TheProject.html">link to the first web page</a>
     once your socket layer works.</p>
</body>
</html>
)HTML";

}

bool http_get(const Url& url, HttpResponse& out, Url& final_url, std::string& err,
              int max_redirects) {
    (void)max_redirects;
    err.clear();
    final_url = url;
    out.status = 200;
    out.headers["content-type"] = "text/html";
    out.body = kBuiltInPage;
    return true;
}