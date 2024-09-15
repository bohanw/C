
#include "circular_buf.h"


// // Opaque circular buffer structure
// typedef struct circular_buf_t circular_buf_t;
// // Handle type, the way users interact with the API
// typedef circular_buf_t* cbuf_handle_t;

// struct circular_buf_t {
// 	u_int8_t * buffer;
// 	size_t head;
// 	size_t tail;
// 	size_t max; //of the buffer
// 	bool  full;
// };


//Goal: want user 
/*
Goal want user to provide the buffer w/ size, when dynamic mem allocation disallowed in 

*/
//---- Adding and Removing Data ----//
#pragma mark - Private func
static void advance_pointer(cbuf_handle_t cbuf){
    assert(cbuf);
    if(circular_buf_full(cbuf)){
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;

    }

    cbuf->head = (cbuf->head + 1) % cbuf->max;

    cbuf->full = (cbuf->head == cbuf->tail);
}

static void  decrement_pointer(cbuf_handle_t cbuf){
    assert(cbuf);
    cbuf-> full = false;
    cbuf-> tail = (cbuf->tail + 1) % cbuf->max;
}
//--------------------------------------
//Return a pointer to a struct instance n
cbuf_handle_t circular_buf_init(uint8_t* buffer, size_t size)
{
	assert(buffer && size);

	cbuf_handle_t cbuf = malloc(sizeof(circular_buf_t));
	assert(cbuf);

	cbuf->buffer = buffer;
	cbuf->max = size;
	circular_buf_reset(cbuf);
	assert(circular_buf_empty(cbuf));

	return cbuf;
}

void circular_buf_reset(cbuf_handle_t cbuf){
    assert(cbuf);
    cbuf->head = 0;
    cbuf-> tail = 0;
    cbuf->full = false;
}
//Destroy container by freeing 
void circular_buf_free(cbuf_handle_t cbuf){
    assert(cbuf);
    free(cbuf);
}
//-----State check-----

bool circular_buf_full(cbuf_handle_t cbuf){
    assert(cbuf);
    return cbuf->full;
}

bool circular_buf_empty(cbuf_handle_t cbuf){
    assert(cbuf);
    return (!(cbuf->full) && (cbuf->head == cbuf->tail));
}

size_t circular_buf_cap(cbuf_handle_t cbuf){
    assert(cbuf);
    return cbuf-> max;
}

size_t circular_buf_size(cbuf_handle_t cbuf){
    assert(cbuf);

    size_t  sz = cbuf->max;
    //two cases: 
    //1) when buf full, size == max, return directly
    //2) when not full, 
    if(!cbuf-> full){
        //consider relative position of head and tail
        if(cbuf-> head >= cbuf->tail){
            sz = (cbuf-> head - cbuf -> tail);
        }
        else {
            sz = cbuf->max + (cbuf-> head - cbuf->tail); 
        }
    }
    return sz;
}

int circular_buf_put(cbuf_handle_t cbuf, uint8_t data){
    assert(cbuf && cbuf->buffer);

    int sts = -1;

    if(!(circular_buf_full(cbuf))){
        cbuf->buffer[cbuf->head] = data;
        advance_pointer(cbuf);
        sts = 0;
    }

    return sts;
}


//----------- Adding and removing Data -----------//

int circular_buf_get(cbuf_handle_t cbuf, uint8_t* data){
    assert(cbuf && data && cbuf->buffer);

    int sts = -1;

    if(!circular_buf_empty(cbuf)){
        *data = cbuf->buffer[cbuf->tail];
        decrement_pointer(cbuf);
        sts = 0;
    }
    return sts;
}