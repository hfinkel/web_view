/*
 *                 Copyright (C) 2018, UChicago Argonne, LLC
 *                            All Rights Reserved
 *
 *                          Software Name: web_view
 *                      By: Argonne National Laboratory
 *                            OPEN SOURCE LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * *****************************************************************************
 *                                 DISCLAIMER
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * *****************************************************************************
 */

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
      virtual std::future<std::string> run_script(const std::string &script) = 0;
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

    std::future<std::string> run_script(const std::string &script) {
      return m->run_script(script);
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

