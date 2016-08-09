import os
env = Environment(ENV = os.environ,
                  TARGET_ARCH = os.environ.get('TARGET_ARCH', 'x86'))
try:
    env.Tool('config', toolpath = [os.environ.get('CBANG_HOME')])
except Exception, e:
    raise Exception, 'CBANG_HOME not set?\n' + str(e)

env.CBLoadTools('compiler cbang dist')
conf = env.CBConfigure()

# Dist
if 'dist' in COMMAND_LINE_TARGETS:
    env.__setitem__('dist_build', '')

    # Only files check in to Subversion
    lines = os.popen('svn status -v').readlines()
    lines += os.popen('svn status -v cbang').readlines()
    lines = filter(lambda l: len(l) and l[0] in 'MA ', lines)
    files = map(lambda l: l.split()[-1], lines)
    files = filter(lambda f: not os.path.isdir(f), files)

    tar = env.TarBZ2Dist('libpcb', files)
    Alias('dist', tar)
    Return()


if not env.GetOption('clean'):
    # Configure compiler
    conf.CBConfig('compiler')

    conf.CBConfig('cbang')
    env.CBDefine('USING_CBANG') # Using CBANG macro namespace

    # Include path
    env.AppendUnique(CPPPATH = ['#/src'])

conf.Finish()

# Source
src = Glob('src/*.cpp') + Glob('src/pcb/*.cpp')

# Build in 'build'
import re
VariantDir('build', 'src')
src = map(lambda path: re.sub(r'^src/', 'build/', str(path)), src)
env.AppendUnique(CPPPATH = ['#/build'])


# Build
lib = env.Library('libpcb', src)
prog = env.Program('pcbtool', ['build/pcbtool.cpp'] + lib)
Default(prog)


# Clean
Clean(prog, ['build', 'config.log', 'dist.txt'])
