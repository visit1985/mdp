#ifndef URL_H
#define URL_H

// init the url module
void url_init(void);

int url_add(const char * link_name, int link_name_length, const char * target, int target_length, int x, int y);
char * url_get_target(int index);
char * url_get_name(int index);
int url_get_amount(void);
void url_purge(void);
void url_dump(void);

#endif // URL_H
