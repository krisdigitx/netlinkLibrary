#include <iostream>
#include <stdio.h>
#include <netlink/netlink.h>
#include <netlink/route/link.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    //cout << "Hello world!" << endl;

    //if (argc != 2){
    //    printf("usage %s interface\n ", argv[0]);
    //}

    int ret = 0;
    struct nl_handle *handle;

    //Allocate and initialize a netlink handle
    handle = nl_handle_alloc();
    if (handle == 0){
        printf("Failed to allocate a netlink handle\n");
        ret = -1;
        return ret;
        //goto error_handle;
    }

    //bind and connect the socket to a protocol
    ret = nl_connect(handle, NETLINK_ROUTE);
    if (ret != 0){
        printf("failed to connect to kernel\n");
        ret = -1;
        nl_handle_destroy(handle);
        //goto error_connect;
    }

    //join a group to receive notifications
    ret = nl_socket_add_membership(handle, RTNLGRP_LINK);
    if (ret != 0){
        printf("joining group failed\n");
        ret = -1;
        nl_handle_destroy(handle);
        //goto error_connect;
    }

    //retrieve list of all interfaces and put them in cache
    struct nl_cache *cache = rtnl_link_alloc_cache(handle);
    if ( cache == 0 ){
        printf("error creating link cache\n");
        ret = -1;
        nl_handle_destroy(handle);
        //goto error_connect;
    }

    //lookup interface
    struct rtnl_link *link = rtnl_link_get_by_name(cache, "ens33");
    if (link == 0){
        printf("No such interface %s\n", "ens33");
        ret = -1;
        nl_cache_free(cache);
        //goto error_cache;
    }


    printf("packets sent %llu\n", rtnl_link_get_stat(link, RTNL_LINK_TX_PACKETS));
    printf("packets received %llu\n",rtnl_link_get_stat(link, RTNL_LINK_RX_PACKETS));




    //give the object back to the cache
    rtnl_link_put(link);

    /*
    error_cache:
        nl_cache_free(cache);
    error_connect:
        nl_handle_destroy(handle);

    error_handle:
        return ret;
    */
    return 0;
}
