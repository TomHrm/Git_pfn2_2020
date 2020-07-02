f <- "uebung_simple/blast_clupea_harengus.out"

names <- c("ID", "startend", "e_val")
b_data <- read.table(file = f, skip = 3, col.names = names)
summary(b_data)
vec <- c(log(as.numeric(b_data$e_val),base = 10))
xlab_text = expression(paste("log"["10"], italic(' e'), '-value'))

nr <- nrow(b_data)
myname <- "AllerBesteCoolsteGruppe"
info = paste(nr, "sequences \n", myname)

png(file="SuperTollesHistogram.png")
hist(vec, xlab = xlab_text, main = info)

dev.off()