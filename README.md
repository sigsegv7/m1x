# The M1X operating system

M1X is an operating system partially inspired by Solaris aiming to provide a pragmatic
capability based security environment. M1X follows the "(almost) everything is a file"
philosophy in which machine resources and files are accessible through a unified file
interface similar to those found in POSIX environments, however, the underlying file
management mechanisms are to be handled orthogonally to machine resource managment.

The way this is to be done is by introducing the concept of filepath prefixes which
allow the kernel to differentiate between machine resources and true files.

## Building M1X

To build M1X, start with obtaining required dependencies by executing the ``bootstrap.sh``
script found within the top-level ``host/`` directory. After which, one may build the toolchain
by invoking ``toolchain.sh`` which is found in the same directory.

Once the toolchain is complete, the world can be built by invoking the ``build.sh`` script located
in the top-level ``host/`` directory.
