#pragma once
#include <IResponse.hpp>
#include <IClient.hpp>
namespace ft
{
	class IResponseSender
	{
	public:
		virtual int sendResponce(IResponse *resp, IClient *client);
		virtual ~IResponseSender() {}
	};
}