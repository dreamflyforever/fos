/*File : http.h 
 *Auth : sjin 
 *Date : 20141206 
 *Mail : 413977243@qq.com 
 */  
#ifndef _MY_HTTP_H  
#define _MY_HTTP_H  
  
#define MY_HTTP_DEFAULT_PORT 80  
  
char * http_get(const char *url);  
char * http_post(const char *url,const char * post_str);  
  
#endif  
