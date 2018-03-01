# snakemake                             # to run
# snakemake --force clear               # to force rule
# snakemake -n                          # check workflow
# REMEMBER REMEMBER : pipe in powershell wraps the object into utf-16 char set (avoid it...evil!)
# WIN32 :=  cmd /C "snakemake --dag | dot -Tpdf > workflow.pdf"
# UNIX  :=  snakemake --dag | dot -Tpdf > workflow.pdf
import numpy as np
import pandas as pd
import os
import seaborn as sns
import matplotlib.pylab as plt
import platform
import shutil
from contextlib import suppress
import matplotlib.patches as mpatches
from sklearn.metrics import confusion_matrix
from collections import Counter


sns.set_context("paper", font_scale=1.5)

def f(x, **kwargs):
    kwargs.pop("color")
    col = next(plt.gca()._get_lines.prop_cycler)['color']
    sns.kdeplot(x, color=col, shade=True, alpha=.5, **kwargs)
    

sep = os.sep
runtest = ["runtest_nn", "runtest_bayes_cls", "runtest_clustering", "runtest_pca", "runtest_fbp", "runtest_rsgd"]
classify = ["diagqda", "qda", "perceptron", "neuralnetwork", "pca"]
cluster = ["kmean", "densityclustering"]
deep = ["rfbp", "rsgd"]

#if platform.system() == "Windows":
#    extension = "exe"
#    build = "powershell .\\build.ps1"
#elif platform.system() == "Linux" or platform.system()[:6] == "CYGWIN":
#    extension = ""
#    build = 'bash -c "./build.sh"'
#else:
#    print("Unsupported OS")
#    sys.exit(1)

with(suppress(OSError)):
    os.makedirs(os.path.join("tst", "res")) 
    os.makedirs("plt")

rule all:
    input:
        #exe          = expand(os.path.join("bin", "{res}." + extension), res=runtest),
        imgs_cls     = expand(os.path.join("plt", "{res}.png"), res=classify),
        imgs_cluster = expand(os.path.join("plt", "{res}.png"), res=cluster),
        imgs_deep    = expand(os.path.join("plt", "{res}.png"), res=deep),

#rule build_program:
#    output:
#        exe = expand(os.path.join("bin", "{program}." + extension), program = runtest)
#    shell:
#        build

rule run_exec:
    input:
        exe = expand(os.path.join("bin", "{program}." + extension), program=runtest),
    output:
        out1 = expand(os.path.join("tst", "res", "{res}.cluster"), res=cluster),
        out2 = expand(os.path.join("tst", "res", "{res}.class"), res=classify),
        out3 = expand(os.path.join("tst", "res", "{res}.deep"), res=deep),
    benchmark:
        os.path.join("benchmark", "run_exec.txt"),
    run:
        for exe in input.exe:
            print("---------------- ", exe , " ----------------")
            os.system(exe)
            print("\n\n")

rule plot_cluster:
    input:
        file = os.path.join("tst", "res", "{algo}.cluster"),
    output:
        img = os.path.join("plt", "{algo}.png"),
    benchmark:
        os.path.join("benchmark", "benchmark_plot_cluster_{algo}.txt")
    run:
        data = pd.read_csv(input.file, sep="\t", header=0)
        legend = []
        fig, ax = plt.subplots(figsize=(10, 8))
        fig.subplots_adjust(left=0.1, right=0.95, top=0.95,  bottom=0.1)
        for i, c in enumerate(pd.unique(data.label)):
            cmap = sns.cubehelix_palette(n_colors=50, 
                                         start=i*.3, 
                                         rot=0, 
                                         light=0.7, 
                                         dark=0,
                                         as_cmap=True)
            legend.append(mpatches.Patch(color=cmap(1), 
                                        label="cluster %d"%i)
                          )
            ax = sns.kdeplot(data[data.label==c].dim0,
                             data[data.label==c].dim1,
                             cmap= cmap, 
                             shade=True,
                             alpha=.6,
                             shade_lowest=False,
                             legend=True)
        ax.legend(handles=legend)
        plt.savefig(output.img)

rule plot_cls:
    input:
        file = os.path.join("tst", "res", "{algo}.class"),
    output:
        img = os.path.join("plt", "{algo}.png")
    benchmark:
        os.path.join("benchmark", "benchmark_plot_cls_{algo}.txt")
    run:
        data = pd.read_csv(input.file, sep="\t", header=0)
        plot =  sns.pairplot(data, diag_kind="kde", markers="o",
                            hue="label",
                            vars=[c for c in data.columns if c != "label"],
                            plot_kws=dict(s=50, edgecolor="k", linewidth=1),
                            aspect=2,
                            )
        plot.map_diag(f)
        plot.fig.subplots_adjust(left=0.1, right=0.95, top=0.95,  bottom=0.1)
        plot.savefig(output.img)

rule plot_deep:
    input:
        file = os.path.join("tst", "res", "{algo}.deep"),
    output:
        img = os.path.join("plt", "{algo}.png"),
    benchmark:
        os.path.join("benchmark", "benchmark_plot_deep_{algo}.txt")
    run:
        data = pd.read_csv(input.file, sep="\t", header=0)
        res = pd.Series(data=confusion_matrix(data["true_label"], data["predict_label"]).ravel(), index=["TP", "FP", "FN", "TN"], name="validation", dtype=np.float)
        occ = Counter(data["true_label"])
        fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(15,8))
        cfs = pd.DataFrame(data=res.values.reshape(2,2), columns=["TP", "FP"], index=["FN", "TN"])
        table = sns.heatmap(cfs, 
                            annot=True, 
                            fmt=".0f", 
                            linewidths=-1, 
                            cmap="coolwarm_r", 
                            annot_kws={"size":14, "weight":"semibold"}, 
                            cbar=150%2, 
                            vmin=res.min().min(), 
                            vmax=res.max().max(),
                            ax=ax2,
                            )
        ax2.set_title("Confusion Matrix", fontsize=14)
        res["TP"] /= occ[-1]
        res["FP"] /= occ[-1]
        res["FN"] /= occ[1]
        res["TN"] /= occ[1]
        res.plot(kind='pie', 
                 subplots=True,
                 labels=list(res.index),
                 colors=['royalblue', 'gold', 'lightgreen', 'darkkhaki'], 
                 autopct='%.2f%%', 
                 fontsize=20, 
                 shadow=True, 
                 startangle=140, 
                 explode=(.02,.02,.02,.02),
                 legend=True,
                 ax=ax1
                 )
        ax1.set_title("Performances", fontsize=14)
        plt.savefig(output.img)

rule clear:
    input:
        build_dir       = "build",
        bin_dir         = "bin",
        benchmark_dir   = "benchmark",
        plt_dir         = "plt",
        res_dir         = os.path.join("tst", "res"),
    run:
        shutil.rmtree(input.build_dir)
        shutil.rmtree(input.bin_dir)
        shutil.rmtree(input.benchmark_dir)
        shutil.rmtree(input.plt_dir)
        shutil.rmtree(input.res_dir)

