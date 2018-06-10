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

#if __cpp_concepts >= 201507
  template <typename T>
  concept
#if __cpp_concepts == 201507
  bool
#endif
  CloseHandler = requires(T handler) {
    { handler() };
  };
#endif

  namespace web_view_detail {
    using HandlerFunc = std::function<bool(const std::string &, std::ostream &)>;
    using CloseHandlerFunc = std::function<void()>;

    struct impl {
      virtual ~impl() { }

      virtual std::future<std::error_code> display_from_uri(const std::string &uri) = 0;
      virtual void register_uri_scheme_handler(const std::string &scheme,
                                               HandlerFunc *func) = 0;
      virtual void register_close_handler(CloseHandlerFunc *func) = 0;
    };

    impl *make_web_view_impl(const std::string &title);
  }

  struct web_view {
    web_view(const std::string &title = "") : m(web_view_detail::make_web_view_impl(title)) { }

    std::future<std::error_code> display_from_uri(const std::string &uri) {
      return m->display_from_uri(uri);
    }

    template <typename URISchemeHandler>
    void set_uri_scheme_handler(const std::string &scheme, URISchemeHandler handler) {
      m->register_uri_scheme_handler(scheme, new web_view_detail::HandlerFunc(handler));
    }

    template <typename CloseHandler>
    void set_close_handler(CloseHandler handler) {
      m->register_close_handler(new web_view_detail::CloseHandlerFunc(handler));
    }

  private:
    std::unique_ptr<web_view_detail::impl> m;
  };
}

#endif // __WEB_VIEW_H__

