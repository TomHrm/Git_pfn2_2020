onesamp <- function(n, true_prob){
  nparty <- length(true_prob)
  s <- sample(1:nparty, n, replace = TRUE, prob = true_prob)
  return (table(s))
}

calculate <- function(n_sampling, n_people, true_prob, parties){
  ms <- replicate(n_sampling, onesamp(n_people, true_prob))
  ms <- ms / n_people
  rownames(ms) <- parties
  return (ms)
}

output <- function(ms, parties, n, n_sampling, file, plot_file){
  cat ('Data collected from ', n, ' people and with ', n_sampling,
       ' resampling\n', file=file, append=TRUE)
  cat('party\texpected\tmedian\t95% limits\n', file=file, append=TRUE)
  
  for (party in parties){
    cat(party, '\t', mean(ms[party,]), '\t', median(ms[party,]), '\t',
        quantile(ms[party,], c(0.025)), ' - ',
        quantile(ms[party,], c(0.975)), '\n', file=file, append=TRUE)
  }
  
  cat('\n', file=file, append=TRUE)
  
  png(filename=plot_file)
  boxplot(ms, use.cols=FALSE, names=parties, main=paste(n, ' people'))
  dev.off()
}

true_prob <- c(0.21, 0.16, 0.29, 0.11)
true_prob <- c(true_prob, 1 - sum(true_prob))
parties <- c('green', 'spd', 'csu', 'afd', 'others')

n_sampling <- 20000
n_people <- c(500, 1000, 2000)

file <- 'table.txt'
cat('', file=file)
for (n in n_people){
  ms <- calculate(n_sampling, n, true_prob, parties)
  output(ms, parties, n, n_sampling, file, paste('SuperTollesPlot', n, '.png'))
}