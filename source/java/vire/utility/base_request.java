package vire.utility;

import java.io.*;
import vire.utility.base_payload;

public class base_request
    extends base_payload
{

    public base_request()
    {
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_request";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BaseRequest";
    }

    @Override
    public final payload_category get_category()
    {
	return payload_category.CATEGORY_REQUEST;
    }

    public static void main(String[] args)
    {
	base_request r = new base_request();
	System.out.printf("Base request : '%s'%n", r.get_category().label());
    }

}
