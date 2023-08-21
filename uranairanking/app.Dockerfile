FROM php:7.3-apache

ENV DEBIAN_FRONTEND noninteractive
RUN apt update && \
    apt install -y zlib1g-dev libzip-dev vim postfix rsyslog


RUN docker-php-ext-install mysqli pdo_mysql zip
RUN curl -s https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin/ --filename=composer
RUN pecl install xdebug-2.9.0

RUN apt update && \
    apt install -y locales && \
    locale-gen ja_JP.UTF-8

ENV BASEDIR /home/uranairank/uranairank00001
ENV APACHE_DOCUMENT_ROOT $BASEDIR/www
RUN sed -ri -e 's!/var/www/html!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/sites-available/*.conf
RUN sed -ri -e 's!/var/www/!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/apache2.conf /etc/apache2/conf-available/*.conf
RUN a2enmod headers && \
    a2enmod rewrite

ARG USERNAME=uranairank00001
ARG GROUPNAME=user
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID $GROUPNAME && \
    useradd -m -s /bin/bash -u $UID -g $GID $USERNAME -d $BASEDIR

RUN cd && \
    mkdir -p uranai_lib/bat/backup && \
    mkdir -p uranai_lib/bat/data_save && \
    mkdir -p uranai_lib/bat/log && \
    mkdir -p uranai_lib/bat/tests && \
    mkdir -p uranai_lib/bat/tmp && \
    mkdir -p uranai_lib/bat/tmpg

USER $USERNAME
WORKDIR $BASEDIR

COPY .bashrc $BASEDIR
COPY .docker/php/xdebug.ini /usr/local/etc/php/conf.d/xdebug.ini
COPY .docker/app/server.php ./uranai_lib/libadmin/server.php
COPY .docker/app/.htaccess ./www/.htaccess

COPY bin ./bin
COPY uranai_lib ./uranai_lib
COPY www ./www

# RUN /etc/init.d/rsyslog start
# RUN /etc/init.d/postfix start
# RUN /etc/init.d/postfix start-fg
