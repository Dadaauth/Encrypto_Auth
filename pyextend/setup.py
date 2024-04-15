from setuptools import setup, Extension

my_extension = Extension(
    name="encrypto_auth",
    sources=["encrypt.c", "decrypt.c",
             "error.c", "tools/gen_sec.c",
             "tools/prepend_filename.c",
             "tools/progressBar.c", "pyextend/pyextend.c"
             ],
    libraries=["crypto"],
    include_dirs=["/usr/include/openssl"],
    
)
setup(name="encrypto_auth",
        version="1.0.0",
        description="Python interface for the encrypto program written in C.",
        author="Dada Clement Authority",
        author_email="dadaauthourity23@gmail.com",
        ext_modules=[my_extension],
        options={'build_ext': {'plat_name': 'linux_i686'}},
        )