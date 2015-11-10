using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NNModule;

namespace GeneticEvolver
{
    class Population
    {
        private static Random random = new Random();
        private List<Controller> _controllers;
        //private Simulation _simulation;
        
        public Controller Best { get { return _controllers.Max(); } }
        public double BestFitness { get { return Best.Fitness; } }
        public double AvgFitness { get { return _controllers.Average(contr => contr.Fitness); } }

        public Population(int popSize)
        {
            _controllers = new List<Controller>(popSize);
            //_simulation = Simulation.CloneDefault();
            int inputCount = Simulation.CloneDefault().SensorStates.Count;
            for (int i = 0; i < popSize; i++)
            {
                NeuralNetwork network = NNFactory.CreateElmanNN(inputCount, 2);
                network.RandomizeWeights(-0.5, 0.5);
                _controllers.Add(new Controller(network));
            }
        }

        public Population(List<Controller> controllers)
        {
            _controllers = controllers;
        }

        public void Evaluate(Func<Simulation, double> evaluator, uint stepsPerContr, uint stepsPerComm)
	    {
            Parallel.ForEach(_controllers, contr =>
            {
                contr.Fitness = 0;
                for (int i = 0; i < stepsPerContr; i++)
                {
                    contr.MoveRobot();
                    contr.Simulation.Update(stepsPerComm);
                    contr.Fitness += evaluator(contr.Simulation);
                }
                contr.Fitness /= stepsPerContr;
                contr.Simulation.ShuffleRobot(stepsPerComm * 10);
            });
	    }

        public Population Select(int n)
        {
            List<Controller> newList = new List<Controller>(_controllers.Count);
            for (int i = 0; i < _controllers.Count; i++)
            {
                List<Controller> challengeList = new List<Controller>(n);
                for (int j = 0; j < n; j++)
                    challengeList.Add(_controllers[random.Next(_controllers.Count)]);
                challengeList.Sort();
                newList.Add(challengeList.Last());
            }

            return new Population(newList);// { _simulation = _simulation };
        }

        public void RouletteWheelSelect()
        {
            List<Controller> result = new List<Controller>(_controllers.Count);
            List<Controller> copy = new List<Controller>(_controllers);
            copy.Sort();
            List<double> shares = copy.Select(contr => contr.Fitness).ToList();
            foreach (double d in shares)
                Console.WriteLine(d);
            double sum = shares.Sum();
            shares.ForEach(share => share = share / sum);
            foreach (double d in shares)
                Console.WriteLine(d);
        }

        public void Crossover(double p)
        {
            _controllers.Shuffle();
            for(int i = 0; i < _controllers.Count / 2; i++)
                if (random.NextDouble() <= p)
                {
                    List<double> weightsA = _controllers[2 * i].NeuralNetwork.GetAllWeights();
                    List<double> weightsB = _controllers[2 * i + 1].NeuralNetwork.GetAllWeights();
                    int flipPoint = random.Next(weightsA.Count);
                    for (int j = 0; j < weightsA.Count; j++)
                    {
                        double temp = weightsA[j];
                        weightsA[j] = weightsB[j];
                        weightsB[j] = temp;
                    }
                    _controllers[2 * i].NeuralNetwork.SetAllWeights(weightsA);
                    _controllers[2 * i + 1].NeuralNetwork.SetAllWeights(weightsB);
                }
        }

        public void Mutate(double p)
	    {
            Random random = new Random();
            double maxMut = 0.5;
            foreach (Controller contr in _controllers)
            {
                List<double> weights = contr.NeuralNetwork.GetAllWeights();
                for(int i = 0; i < weights.Count; i++)
                {
                    if (random.NextDouble() <= p)
                        weights[i] += -maxMut + random.NextDouble() * maxMut * 2;
                }
                contr.NeuralNetwork.SetAllWeights(weights);

            }
	    }
    }

    static class MyExtensions
    {
        private static Random random = new Random();

        public static void Shuffle<T>(this IList<T> list)
        {
            int n = list.Count;
            while (n > 1)
            {
                n--;
                int k = random.Next(n + 1);
                T value = list[k];
                list[k] = list[n];
                list[n] = value;
            }
        }
    }
}
