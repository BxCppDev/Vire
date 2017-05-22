package vire.utility;

import java.io.*;
import vire.utility.base_payload;

public class base_response
    extends base_payload
{

    public base_response()
    {
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_response";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BaseResponse";
    }

    @Override
    public final payload_category get_category()
    {
	return payload_category.CATEGORY_RESPONSE;
    }

    public static void main(String[] args)
    {
	base_response r = new base_response();
	System.out.printf("Base response : '%s'%n", r.get_category().label());
    }

}
