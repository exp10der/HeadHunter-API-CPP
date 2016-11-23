#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

using namespace utility; // Common utilities like string conversions
using namespace web; // Common features like URIs.
using namespace web::http; // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace concurrency::streams; // Asynchronous streams

int main()
{
	std::wstring resumeId = L"your resumeId";
	std::wstring accessToken = L"your accessToken";

	http_client client(U("https://api.hh.ru"));

	http_request request(methods::GET);
	request.headers().add(L"Authorization", L"Bearer " + accessToken);
	request.headers().add(L"UserAgent", L"Exp10der Resume Prolongation/1.0 (konstantin.writing.code@gmail.com)");
	request.set_request_uri(L"/resumes/" + resumeId);


	client
		.request(request)
		.then([](http_response response)-> pplx::task<json::value>
			{
				if (response.status_code() == status_codes::OK)
				{
					return response.extract_json();
				}

				return pplx::task_from_result(json::value());
			})
		.then([](pplx::task<json::value> previousTask)
			{
				try
				{
					json::value const& v = previousTask.get();
					auto nextPublishAt = v.at(L"next_publish_at").as_string();
					auto dateTime = datetime::from_string(nextPublishAt, datetime::ISO_8601);
					std::wcout << "Time to update the resume: " << dateTime.to_string() << std::endl;
				}
				catch (http_exception const& e)
				{
					std::wcout << e.what() << std::endl;
				}
			})
		.wait();


	http_request requestUpdate(methods::POST);
	requestUpdate.headers().add(L"Authorization", L"Bearer " + accessToken);
	requestUpdate.headers().add(L"UserAgent", L"Exp10der Resume Prolongation/1.0 (konstantin.writing.code@gmail.com)");
	requestUpdate.set_body(L"");
	requestUpdate.set_request_uri(L"/resumes/" + resumeId + L"/publish");


	client
		.request(requestUpdate)
		.then([](http_response response)-> pplx::task<json::value>
			{
				if (response.status_code() == status_codes::OK)
				{
					auto test = response.extract_json();
					return response.extract_json();
				}

				return pplx::task_from_result(json::value());
			})
		.then([](pplx::task<json::value> previousTask)
			{
				try
				{
					json::value const& v = previousTask.get();
					auto contents = v.serialize();
					std::wcout << "Update Response:  " << contents << std::endl;
				}
				catch (http_exception const& e)
				{
					std::wcout << e.what() << std::endl;
				}
			})
		.wait();

	return 0;
}
