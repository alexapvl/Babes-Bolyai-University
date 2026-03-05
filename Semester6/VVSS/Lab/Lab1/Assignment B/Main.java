/*
Problem statement. Compute the longest sequence such that there exists at most 3 different values.

Problem design: The program must have: a subalgorithm that reads the sequence, a subalgorithm that computes the first position and the
length of the longest sequence; a subalgorithm that prints the longest sequence found
*/
public class Main {
    public static void main(String[] args) {
        int[] valE = {1, 3, 3, 2, 3};
        int nrE = valE.length - 1;
        Result rezultat = ComputeMaxSeq(nrE, valE);
        System.out.println("pozF=" + rezultat.pozF + ", lungF=" + rezultat.lungF);
    }

    // Subalgorithm ComputeMaxSeq(nrE,valE,pozF,lungF)
    static Result ComputeMaxSeq(int nrE, int[] valE) {
        int i = 0;
        int pozF = 0;
        int lungF = 0;
        while (i <= nrE) {
            int lungFinala = ComputeASequence(i, nrE, valE);
            if (lungFinala > lungF) {
                pozF = i;
                lungF = lungFinala;
            } else {
                i = i + 1;
            }
        }
        return new Result(pozF, lungF);
    }

    // Subalgorithm ComputeASequence (pozStart,nrE,valE,lungFinala)
    static int ComputeASequence(int pozStart, int nrE, int[] valE) {
        int lungFinala = 0;
        int pozStar = pozStart; // reflects the original typo (pozStar vs pozStart)
        int i = pozStart;
        while ((pozStart <= nrE) && Still3Values(valE[i])) {
            lungFinala = lungFinala + 1;
            pozStart = pozStar + 1;
        }
        return lungFinala;
    }

    static boolean Still3Values(int value) {
        return value == 3;
    }

    static class Result {
        final int pozF;
        final int lungF;

        Result(int pozF, int lungF) {
            this.pozF = pozF;
            this.lungF = lungF;
        }
    }
}

