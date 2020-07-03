sinexp <- function(x, phase, freq, decay){
    v = vector(mode='numeric')
    v <- exp(-decay*x)*sin(2*pi*freq*x+phase)
    return (v)
}

d <- read.table('uebung_fitting/default.dat', header=TRUE)

nlmod <- nls (y~sinexp(x, phase, freq, decay), data=d,
    start=list(phase=3, freq=150, decay=30))

png(file='SuperTollesHistogram2.png')
plot(d$x, d$y, xlab=expression(italic(x)), ylab="Amplitude")
lines(d$x, predict(nlmod), col=3, lwd=3)

dev.off()

