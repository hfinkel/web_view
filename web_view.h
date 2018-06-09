// Author: Hal Finkel <hfinkel@anl.gov>

#ifndef __WEB_VIEW_H__
#define __WEB_VIEW_H__

#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <iostream>
#include <future>
#include <system_error>

namespace wv {    
#if __cpp_concepts >= 201507
  template <typename T>
  concept
#if __cpp_concepts == 201507
  bool
#endif
  URISchemeHandler = requires(T handler, const std::string &uri, std::ostream &os) {
    { handler(uri, os) } -> std::error_code;
  };
#endif

  namespace web_view_detail {
    struct impl {
      virtual ~impl() { }

      virtual std::future<std::error_code> display_html_from_uri(const std::string &uri) = 0;
      virtual std::future<std::error_code> display_html(std::string_view html,
                                                        const std::string &effective_uri) = 0;

      using HandlerFunc = std::function<bool(const std::string &, std::ostream &)>;
      virtual bool register_uri_scheme_handler(const std::string &scheme,
                                               const HandlerFunc &func) = 0;
    };

    impl *make_web_view_detail(const std::string &title);
  }

  struct web_view {
    web_view(const std::string &title = "") : m(make_web_view_detail(title)) { }

    std::future<std::error_code> display_html_from_uri(const std::string &uri) {
      return m->display_html_from_uri(uri);
    }

    std::future<std::error_code> display_html(std::string_view html,
                                              const std::string &effective_uri) {
      return m->display_html(html, effective_uri);
    }

    template <typename URISchemeHandler>
    bool set_uri_scheme_handler(const std::string &scheme, URISchemeHandler handler) {
      return m->register_uri_scheme_handler(scheme, handler);
    }

  private:
    std::unique_ptr<web_view_detail::impl> m;
  };
}

#endif // __WEB_VIEW_H__
