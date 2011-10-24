require 'rubygems'
require 'rbconfig'

# Extension task

require 'rake/extensiontask'

Rake::ExtensionTask.new 'kg' do |ext|
  ext.tmp_dir = 'build'
  ext.lib_dir = 'lib/mg'
  ext.cross_compile = true
  ext.cross_platform = [ 'i386-mingw32' ]
end

task default: :compile
