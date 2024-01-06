#include <exec/types.h>
#include <devices/timer.h>

void delete_timer  (struct timerequest *);
struct timerequest *create_timer( ULONG );
LONG get_sys_time  (struct timeval *);

struct timeval mytimeval;
LONG time = 0;
LONG prevTime = 0;

void init_timer(void){
    get_sys_time( &mytimeval );
    time = mytimeval.tv_secs * 1000 + mytimeval.tv_micro / 1000;
    prevTime = time;
}
void mark_timer(BOOL andPrint){
    get_sys_time( &mytimeval );
    time = mytimeval.tv_secs * 1000 + mytimeval.tv_micro / 1000;
    if (andPrint) printf("elapsed: %d\n", time - prevTime);
    prevTime = time;
}
LONG get_timer(void){
    LONG ElapsedTime;
    get_sys_time( &mytimeval );
    time = mytimeval.tv_secs * 1000 + mytimeval.tv_micro / 1000;
    ElapsedTime = time - prevTime;
    prevTime = time;
    return ElapsedTime;
}


LONG get_sys_time(struct timeval *tv){
    struct timerequest *tr;
    tr = create_timer( UNIT_MICROHZ );

    /* non zero return says error */
    if (tr == 0 )
        return( -1 );

    tr->tr_node.io_Command = TR_GETSYSTIME;
    DoIO((struct IORequest *) tr );

    /* structure assignment */
    *tv = tr->tr_time;

    delete_timer( tr );
    return( 0 );
}

struct timerequest *create_timer( ULONG unit )
{
    /* return a pointer to a timer request.  If any problem, return NULL */
    LONG error;
    struct MsgPort *timerport;
    struct timerequest *TimerIO;

    timerport = CreatePort( 0, 0 );
    if (timerport == NULL )
        return( NULL );

    TimerIO = (struct timerequest *)
        CreateExtIO( timerport, sizeof( struct timerequest ) );
    if (TimerIO == NULL )
        {
        DeletePort(timerport);   /* Delete message port */
        return( NULL );
        }

    error = OpenDevice( TIMERNAME, unit,(struct IORequest *) TimerIO, 0L );
    if (error != 0 )
        {
        delete_timer( TimerIO );
        return( NULL );
        }
    return( TimerIO );
}

void delete_timer(struct timerequest *tr ){
    struct MsgPort *tp;

    if (tr != 0 ){
        tp = tr->tr_node.io_Message.mn_ReplyPort;

        if (tp != 0) DeletePort(tp);

        CloseDevice( (struct IORequest *) tr );
        DeleteExtIO( (struct IORequest *) tr );
        }
}
