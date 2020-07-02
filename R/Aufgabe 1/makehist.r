f <- 'blast_clupea_harengus.out'

names <- c('id', 'startend', 'e_val')
b_data <- read.table(file=f, skip=3, col.names=names, header=TRUE)
vec <- log10(b_data$e_val)

nr <- nrow(b_data)
myname <- "AllerBesteCoolsteGruppe"
info = paste(nr, " sequences \n", myname)

xlab_text = expression(paste("log"["10"], italic('e'), '-value'))
png(file="SuperTollesHistogram.png")
hist(vec, xlab = xlab_text, main = info)

dev.off()
