# Use Docker to create RPMS for LROSE distributions

The scripts for these operations are check in under:

  build/packages/centos

1. [Provision](#provision)
2. [Build](#build)
3. [Make RPMs](#make-rpm)

<a name="provision"/>

## 1. Provisioning

This step creates containers with the required packages installed
so that a build will complete successfully.

The scripts are:

  * make_image.centos6.custom
  * make_image.centos7.custom

The resulting custom containers are:

```
  REPOSITORY          TAG        
  centos6             custom
  centos7             custom
```

<a name="build"/>

## 2. Building

This step builds the lrose packages in the provsioned containers.

The scripts are:

  * do_build.centos6.blaze
  * do_build.centos7.blaze

The resulting custom containers are:

```
  REPOSITORY          TAG        
  centos6             blaze
  centos7             blaze
```

<a name="make-rpm"/>

## 3. Make RPMs

This step creates the RPMs from the files in the build containers.

The scripts are:

  * create_rpm.centos6.blaze
  * create_rpm.centos7.blaze

The RPMs are copied out to the host in:

```
  /tmp/centos6-blaze/rpms/x86_64/lrose-blaze-20190119.x86_64.rpm
  /tmp/centos7-blaze/rpms/x86_64/lrose-blaze-20190119.x86_64.rpm
```