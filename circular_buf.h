
// A C implementation of circular buffer
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdbool.h>
typedef u_int8_t uint8_t;

struct circular_buf_t {
	uint8_t * buffer;
	size_t head;
	size_t tail;
	size_t max; //of the buffer
	bool  full;
};

// Opaque circular buffer structure
typedef struct circular_buf_t circular_buf_t;
// Handle type, the way users interact with the API
typedef circular_buf_t* cbuf_handle_t;

cbuf_handle_t circular_buf_init(uint8_t* buffer, size_t size);
void circular_buf_free(cbuf_handle_t cbuf);

void circular_buf_reset(cbuf_handle_t cbuf);

//add new data to buffer
//reject new data if buffer full
int  circular_buf_put(cbuf_handle_t cbuf, uint8_t data);

int circular_buf_get(cbuf_handle_t cbuf, uint8_t * data);

bool circular_buf_empty(cbuf_handle_t cbuf);

bool circular_buf_full(cbuf_handle_t cbuf);

size_t circular_buf_cap(cbuf_handle_t cbuf);

//Get current element size of circular buffer
size_t circular_buf_size(cbuf_handle_t cbuf);


