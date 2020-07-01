# 20 June 2019
# These are what I used for my lecture on Bayes and distributions
# Example based on estimating notes in circulation
# We label 1000 notes
# Sample "n_resampled" notes

# Given a pool of notes, do a single sampling from it.
# count the number of times we see a marked note
single_sample <- function (pool_of_notes, n_resampled) {
     samp <- sample (pool_of_notes, n_resampled)
     return (sum(samp))
}

# sample_notes makes a pool of notes. It then samples from it
# and returns an array. Each element is the number of marked
# notes that were found.
sample_notes <- function (n_notes, n_marked, n_trials, n_resampled) {
     pool_of_notes <- rep (c(1, 0), c(n_marked, n_notes - n_marked))
     return (replicate (n_trials, single_sample(pool_of_notes, n_resampled)))
}

# ------------ main -----------------
mymain <- function () {
     n_marked    <- 1000   # Originally marked this many notes
     n_resampled <- 1000   # We sampled this many notes
     n_found     <- 20     # and found this many marked
     n_trials    <- 1000   # How many times do we repeat sampling
     h <- seq(20000, 120000, 5000) # An array of hypotheses, values to try out     
     set.seed (37)         # I like determinism
     hard_copy = F

     success <- vector()
     under = n_found - (0.02 * n_found)
     over  = n_found + (0.02 * n_found)
     for (i in h) {
          if (i > 70000) {
               t <- 0
          } else {
               a <- sample_notes(i, n_marked, n_trials, n_resampled)
               t <- length(a[ (a >= under) & (a <= over)])
          }
          success <- c(success, t)
     }

     success = success / sum (success)
     tt = expression(paste(italic("n "), 'notes / 1000'))
     if (hard_copy) { svg(filename='note_7000.svg')}
     barplot (success, names = h/1000, xlab=tt, ylab='frequency', las=1)
     if (hard_copy) { dev.off() }
     if (T) { # setting this to T gives us the points on top of the bar plot
          cs <- cumsum(success)
          if (hard_copy) {svg ('cum7000.svg') }
          par(new=T)
          plot (h/1000, cs, ylab = '', xlab='', ylim=c(0,1), axes=F)
          if (hard_copy) {dev.off()}
     }
}
mymain ()
