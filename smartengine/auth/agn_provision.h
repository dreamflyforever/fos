#ifndef __AGN_PROVISION_H__
#define __AGN_PROVISION_H__
typedef int  (*agn_provision_http_request)(void *userdata, const char *url, char response[1024]);
int agn_provision_check_auth(const char *auth_info, char **error_info);
int agn_provision_do_auth(const char *auth_info, char **error_info, agn_provision_http_request request, void *userdata);
#endif
