#pragma once
#include <ABuildPolicy.hpp>
namespace ft
{

	class GetBuildPolicy : public ABuildPolicy
	{
	private:
		GetBuildPolicy(const GetBuildPolicy &ref);
	public:
		GetBuildPolicy();
		virtual ~GetBuildPolicy();
		GetBuildPolicy	&operator=(const GetBuildPolicy &ref);
		IResponse		*buildResponse(IRequest *request);

		void ApplyConfig(ServerInit *server);
	};

} // namespace ft