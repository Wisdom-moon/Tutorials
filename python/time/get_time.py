import datetime

start_time = datetime.datetime.now()
print "hello"
end_time = datetime.datetime.now()

dur_time = end_time - start_time

print dur_time.total_seconds()
