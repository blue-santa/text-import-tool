def Settings( **kwargs ):
  return {
    'flags': [ '-x', 'c++', '-Wall', '-Wextra', '-Werror', '-std=c++17', '-g', 'I./include', '-DGL_SILENCE_DEPRECATION' ],
  }
