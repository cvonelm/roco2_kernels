args = commandArgs(trailingOnly=TRUE)

if(length(args) != 1)
{
    stop("Give the roco2_kernels/scaling make-table experiment output!");
}

print(args[1])

require("tidyverse")

df = read_csv(args[1])

ggplot(df,  aes(x=timestamp, y=power, color=KERNEL)) + geom_point()
ggsave("scaling_roco2.png")
