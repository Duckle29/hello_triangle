project('HelloTriangle',
        'cpp',
        default_options: ['cpp_std=c++14', 'buildtype=debugoptimized', 'warning_level=3'])

dep_glut = dependency('freeglut')
dep_glew = dependency('GLEW') 
dep_glu = dependency('glu')
dep_gl = dependency('gl')
cpp_args = ['-fpermissive']
executable('main', 
           'hellotriangle.cpp', 
           dependencies : [dep_glut, 
                           dep_glew, 
                           dep_glu, 
                           dep_gl], 
           cpp_args : cpp_args) 