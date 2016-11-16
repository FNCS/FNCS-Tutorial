time_current = 0;
time_stop = 10;
fncs_initialize();
while time_current < time_stop
    fncs_publish('object.attribute', 'value');
    fncs_publish('this.will.be.dropped', 'value');
    fncs_route('endpointX', 'endpointY', 'key', 'value');
    all_events = fncs_get_events()
    for event = all_events
        disp('printing events')
        disp(event)
    end
    time_desired = time_current + 1;
    time_granted = fncs_time_request(time_desired);
    time_current = time_granted;
end
fncs_finalize()
