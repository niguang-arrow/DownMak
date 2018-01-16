import subprocess

# subprocess.call(['ls', '-al'])
# subprocess.call('echo $HOME', shell=True)
# try:
    # subprocess.check_call(['false'])
# except subprocess.CalledProcessError as err:
    # print "Error: ", err


# output = subprocess.check_output(['ls'])
# print type(output)
# print len(output)
# print repr(output)

# try:
    # output = subprocess.check_output(
        # 'echo to stdout; echo to stderr 1>&2; exit 1',
        # shell=True,
        # stderr=subprocess.STDOUT
    # )
# except subprocess.CalledProcessError as err:
    # print 'ERROR:', err
# else:
    # print 'Have %d bytes in output' % len(output)
    # print output


# subprocess.call([
    # 'xelatex',
    # 'test.tex'
    # ],
    # stdout=subprocess.PIPE,
# )


# print 'read:'
# proc = subprocess.Popen(
    # ['echo', '"to stdout"'],
    # stdout=subprocess.PIPE,
# )
# stdout_value = proc.communicate()[0]
# print '\tstdout:', repr(stdout_value)


# print 'write:'
# proc = subprocess.Popen(
    # ['cat', '-'],
    # stdin=subprocess.PIPE
# )
# proc.communicate('\tstdin: to stdin\n')


# print 'popen2:'
# proc = subprocess.Popen(
    # ['cat', '-'],
    # stdin=subprocess.PIPE,
    # stdout=subprocess.PIPE
# )

# stdout_value = proc.communicate('\tstdin: to stdin\n')[0]
# print repr(stdout_value)


# print 'popen3'
# proc = subprocess.Popen(
    # 'cat -; echo "to stderr" 1>&2',
    # shell=True,
    # stdin=subprocess.PIPE,
    # stdout=subprocess.PIPE,
    # stderr=subprocess.PIPE
# )

# stdout_value, stderr_value = proc.communicate("hello")
# print repr(stdout_value)
# print repr(stderr_value)


# print 'popen4'
# proc = subprocess.Popen(
    # 'cat -; echo "to stderr" 1>&2',
    # shell=True,
    # stdin=subprocess.PIPE,
    # stdout=subprocess.PIPE,
    # stderr=subprocess.STDOUT
# )

# stdout_value, stderr_value = proc.communicate("hello")
# print repr(stdout_value)
# print repr(stderr_value)


# ls = subprocess.Popen(['ls'],
        # stdout=subprocess.PIPE 
    # )

# grep  = subprocess.Popen(
    # ['grep', 'te'],
    # stdin=ls.stdout,
    # stdout=subprocess.PIPE,
# )

# end_pipe = grep.stdout

# for line in end_pipe:
    # print repr(line)
