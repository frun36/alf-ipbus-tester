from ubuntu
RUN apt-get update -y
RUN apt-get install -y cmake git gcc wget unzip g++ systemctl

RUN wget https://dim.web.cern.ch/dim_v20r37.zip
RUN unzip -a dim_v20r37.zip
RUN mkdir /usr/local/include/dim
RUN cp dim_v20r37/linux/*.so /usr/local/lib/
RUN cp dim_v20r37/linux/*.a /usr/local/lib/
RUN cp dim_v20r37/linux/dns /usr/local/bin/
RUN cp dim_v20r37/dim/* /usr/local/include/dim
RUN rm -rf dim_v20r37

WORKDIR /
RUN wget https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.gz
RUN tar xvzf boost_1_83_0.tar.gz

WORKDIR /boost_1_83_0
RUN ./bootstrap.sh --prefix=/usr/
RUN ./b2
RUN ./b2 install
WORKDIR /
RUN rm -rf boost_1_83_0

WORKDIR /home

RUN git clone https://github.com/frun36/alf-ipbus-tester.git

WORKDIR /home/alf-ipbus-tester
RUN git submodule update --init --recursive
RUN mkdir build
RUN cmake -S . -B build/
RUN cmake --build build
