#include "observable/definitions.h"

#include "disposable/compositedisposable.h"
#include "disposable/disposable.h"

#include "internal/godot.h"

namespace rx::observable {

rx_observable_t godot::from_http_request_(const String& url, const Variant& request_data, bool raw, const String& encoding, HTTPRequest* requester, const PackedStringArray& custom_headers, bool tls_validate_domain, HTTPClient::Method method) {
    
    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

        gd_signal_scheduler_t _scheduler = std::dynamic_pointer_cast<GodotSignalSchedulerBase>(scheduler);
        _scheduler = _scheduler ? _scheduler : GodotSignalScheduler::singleton();

        HTTPRequest* _requester = requester ? requester : memnew(HTTPRequest);
        if (!requester) {
            _requester->set_name("GodotRx$HTTPRequest");
            GDRX->get_root()->add_child(_requester);
        }
        auto n_bytes = _requester->get_downloaded_bytes();

        auto action = [=](const Array& args) -> Variant {
            int result = args[0];
            int response_code = args[1];
            PackedStringArray headers = args[2];
            PackedByteArray body = args[3];

            if (!requester) {
                _requester->queue_free();
            }

            if (_requester->get_downloaded_bytes() - n_bytes == 0) {
                observer->on_error(std::make_exception_ptr(HttpRequestFailedException("No data received")));
                return VNULL;
            }

            Ref<RxHttpRequestResult> http_request_result = memnew(RxHttpRequestResult(
                result,
                response_code,
                headers,
                body
            ));

            if (!encoding.is_empty()) {
                if (encoding == "ascii") {
                    http_request_result->decoded = body.get_string_from_ascii();
                }
                else if (encoding == "utf8") {
                    http_request_result->decoded = body.get_string_from_utf8();
                }
                else if (encoding == "utf16") {
                    http_request_result->decoded = body.get_string_from_utf16();
                }
                else if (encoding == "utf32") {
                    http_request_result->decoded = body.get_string_from_utf32();
                }
                else {
                    observer->on_error(std::make_exception_ptr(rx::exception::BadArgumentException("Unknown encoding: " + std::string(encoding.ascii().get_data()))));
                    return VNULL;
                }
            }

            observer->on_next(http_request_result);
            observer->on_completed();
            return VNULL;
        };

        dispose_t dispose = [=]() {
            if (!requester) {
                _requester->cancel_request();
                _requester->queue_free();
            }
        };

        Error error_code;
        error_code = raw ? 
            _requester->request_raw(url, custom_headers, method, request_data) :
            _requester->request(url, custom_headers, method, request_data);
        if (error_code) {
            dispose();
            observer->on_error(std::make_exception_ptr(HttpRequestFailedException("Unable to create request")));
            return std::make_shared<Disposable>();
        }

        auto subscription = _scheduler->schedule_signal(_requester, "request_completed", action);
        auto cd = std::make_shared<CompositeDisposable>(
            std::make_shared<Disposable>(dispose),
            subscription
        );
        return cd;
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable