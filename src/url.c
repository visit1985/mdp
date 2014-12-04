#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "url.h"

typedef struct Url {
	char * link_name;
	char * target;
	int x;
	int y;
	struct Url * next;
} Url_t;

static void url_del_elem(Url_t * elem);
static void url_print(Url_t * u);

static Url_t *list;
static int index_max;
static int init_ok;

void url_init(void) {
	list = NULL;
	index_max = 0;
	init_ok = 1;
}

int url_add(const char * link_name, int link_name_length, const char * target, int target_length, int x, int y) {
	if (!init_ok) return -1;
	
	Url_t *tmp = NULL;
	int i = 0;
	
	if (list) {
		tmp = list;
		while (tmp->next) {
			tmp = tmp->next;
			i++;
		}
		tmp->next = malloc(sizeof(Url_t));
		assert(tmp->next);
		tmp = tmp->next;
	} else {
		list = malloc(sizeof(Url_t));
		tmp = list;
		assert(tmp);
	}
	
	tmp -> link_name = calloc(link_name_length+1, sizeof(char));
	assert(tmp->link_name);
	strncpy(tmp->link_name, link_name, link_name_length);
	tmp->link_name[link_name_length] = '\0';
	
	tmp->target = calloc(target_length+1, sizeof(char));
	assert(tmp->target);
	strncpy(tmp->target, target, target_length);
	tmp->target[target_length] = '\0';
	
	tmp->x = x;
	tmp->y = y;
	tmp->next = NULL;
	
	index_max++;
	
	return index_max-1;
}


char * url_get_target(int index) {
	if (!init_ok) return NULL;
	
	Url_t * tmp = list;
	
	if (!tmp) return NULL;
	
	while (index > 0 && tmp && tmp->next) {
		tmp = tmp->next;
		index --;
	}
	
	if (!index) {
		return tmp->target;
	} else return NULL; 
}

char * url_get_name(int index) {
	Url_t * tmp = list;
	
	while (index > 0 && tmp && tmp->next) {
		tmp = tmp->next;
		index --;
	}
	
	if (!index) {
		return tmp->link_name;
	} else return NULL; 
}

void url_purge() {
	url_del_elem(list);
	list = NULL;
	index_max = 0;
	init_ok = 0;
}

static void url_del_elem(Url_t *elem) {
	if (!elem) return;
	
	if (elem->next) {
		url_del_elem(elem->next);
		elem->next = NULL;
	}
	
	if (elem->target) {
		free(elem->target);
		elem->target = NULL;
	}
	
	if (elem->link_name) {
		free(elem->link_name);
		elem->link_name = NULL;
	}
	
	free(elem);
}

void url_dump(void) {
	if (!list) return;
	
	Url_t *tmp = list;
	
	while (tmp) {
		url_print(tmp);
		if (tmp->next)
			tmp = tmp->next;
		else break;
	}
}

static void url_print(Url_t * u) {
	printf("Url_t @ %p\n", u);
}

int url_get_amount(void) {
	return index_max;
}

int url_count_inline(const char *line, size_t line_len) {
	int count = 0;
	const char* i = line;
	
	for (; *i; i++) {
		if (*i == '\\') {
			i++;
		} else if ( *i == '[' && *(i+1) != ']') {
			while (*i && (i - line) < line_len && *i != ']') i++;
			i++;
			if (*i == '(' && strchr(i, ')')) {
				count ++;
				i = strchr(i, ')') + 1;
			}
		}
		
		if ((i - line) >= line_len) break;
	}
		
	return count;
}
