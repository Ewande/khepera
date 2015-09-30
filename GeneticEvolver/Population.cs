using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class Population
    {
        private List<Controller> _controllers;
        public Controller Best { get { return _controllers.Max(); } }

        public Population(int popSize)
        {
            _controllers = new List<Controller>(popSize);
            int inputCount = Simulation.CloneDefault().SensorStates.Count;
            for (int i = 0; i < popSize; i++)
            {
                NeuralNetwork network = NNFactory.CreateElmanNN(inputCount, 2, Functions.Sigmoid);
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
            Simulation simulation = Simulation.CloneDefault();
            foreach (Controller contr in _controllers)
            {
                contr.Fitness = 0;
                for (int i = 0; i < stepsPerContr; i++)
                {
                    contr.MoveRobot(simulation);
                    simulation.Update(stepsPerComm);
                    contr.Fitness += evaluator(simulation);
                }
                contr.Fitness /= stepsPerContr;
                simulation.ShuffleRobot(stepsPerComm * 10);
            }
	    }

        public Population Select(int n)
        {
            List<Controller> newList = new List<Controller>(_controllers.Count);
            for (int i = 0; i < _controllers.Count; i++)
            {
                Random rand = new Random();
                List<Controller> challengeList = new List<Controller>(n);
                for (int j = 0; j < n; j++)
                    challengeList.Add(_controllers[rand.Next(_controllers.Count)]);
                challengeList.Sort();
                newList.Add(challengeList.Last());
            }

            return new Population(newList);
        }

        public void Crossover(double p)
        {
            _controllers.Shuffle();
            Random random = new Random();
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
        public static void Shuffle<T>(this IList<T> list)
        {
            Random random = new Random();
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
