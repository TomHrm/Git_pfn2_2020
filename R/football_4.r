# 4 July
# For the lectures, write a counter for football results.
# We assume the number of goals comes from a poisson distribution
rm (list=ls())
# ------- constants

team1_mu <- 1.  # Average number goals per match
team2_mu <- 1.3

hardcopy <- 'F'  # set this to true if the results should go in a file

n_games <- 100000  # How many games to play
seed <- 1787        # Seed for random numbers
# -------- main
set.seed (seed)
team1 <- rpois(n_games, team1_mu)
team2 <- rpois(n_games, team2_mu)

w1 <- sum (team1 > team2)
w2 <- sum (team2 > team1)
draw <- n_games - (w1 + w2)
s <- "team %d won %d (%.0f %%)\n"
t <- "and there were %d (%.0f %%) draws\n"
a <- paste ("From ", n_games, " games\n",
            sprintf (s, 1, w1, 100 * w1/n_games),
            sprintf (s, 2, w2, 100 * w2/n_games),
            sprintf (t, draw, 100 * draw/n_games),
            "average of 1 was ", team1_mu, "\n average of 2 was ", team2_mu
            )
cat (a)

if (hardcopy) { svg ('football4.svg', bg='transparent' )} 

oldpar <- par(no.readonly = TRUE)
breaks=seq(from = -0.5, to = max(c(team1, team2))+0.9 , by = 1)
colour1 = rgb(0, 0, 1, 0.7)
colour2 = rgb(1, 0, 0, 0.7)

xlim=c(0,max(team1, team2))
t <- table(c(team1, team2))
ymax = (max(t)/sum(t)) * 1.1
xlab = "n goals"
par (lab=c(10, 5, 7))
par(fig=c(0, 1, 0.52,1))
h1 <- hist (team1, breaks=breaks, probability=T, main="", col=colour1,
            xlab= "", ylab = "frequency", xlim = xlim, ylim=c(0, ymax), xaxt="n")
legend(x=1.6, y=0.37, legend=c("team1", "team2"), col=c(colour1, colour2), lwd=5, box.lty=0)
text(a, x=4, y=0.3, adj=0)
par(fig=c(0, 1, 0.3, 0.78), new=T)
h2 <- hist (team2, breaks=breaks, probability=T, main="", col=colour2,
            xlab = "", ylab = "", xlim=xlim, ylim=c(ymax,0))

#---------------------------
par(fig=c(0, 1, 0.0, 0.45), new=T)
df = data.frame (team1, team2)
boxplot (df, horizontal=T, ylim=c(0,8), col=c(colour1, colour2),
         xlab= "n goals", frame.plot=F, names=c("t1", "t2"))
par(oldpar)
if (hardcopy) { dev.off() }
