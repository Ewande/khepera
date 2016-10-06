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
        public static int GA_CONST_START = 1;
        public static int GA_HYBRID = 2;

        private static Random random = new Random();
        private List<Controller> _controllers;
        private int gaMode;
        private int iteration = 0;
        //private Simulation _simulation;
        
        public Controller Best { get { return _controllers.Max(); } }
        public double AvgFitness { get { return _controllers.Average(contr => contr.Fitness); } }

        // ------ for research:
        public double AvgSpeedFactor { get { return _controllers.Average(contr => contr.SpeedFactor); } }
        public double AvgMovementFactor { get { return _controllers.Average(contr => contr.MovementFactor); } }
        public double AvgProximityFactor { get { return _controllers.Average(contr => contr.ProximityFactor); } }
        // --------------------

        public Population(int popSize, int _gaMode)
        {
            _controllers = new List<Controller>(popSize);
            gaMode = _gaMode;
            //_simulation = Simulation.CloneDefault();
            int inputCount = Simulation.CloneDefault().SensorStates.Count;
            for (int i = 0; i < popSize; i++)
            {
                NeuralNetwork network = NNFactory.CreateElmanNN(inputCount, 2);
                network.RandomizeWeights(-0.5, 0.5);
                _controllers.Add(new Controller(network));
            }
        }

        public Population(List<Controller> controllers, int _iteration)
        {
            _controllers = controllers;
            iteration = _iteration;
        }

        public void Evaluate(Func<Simulation, Controller, double> evaluator, uint stepsPerContr, uint stepsPerComm)
	    {
            /*foreach(Controller contr in _controllers) */Parallel.ForEach(_controllers, contr =>
            {
                double fitness = contr.Fitness;
                double speedFactor = contr.SpeedFactor;
                double movementFactor = contr.MovementFactor;
                double proximityFactor = contr.ProximityFactor;
                contr.Fitness = 0;
                contr.SpeedFactor = 0;
                contr.MovementFactor = 0;
                contr.ProximityFactor = 0;

                for (int i = 0; i < stepsPerContr; i++)
                {
                    contr.MoveRobot(/*_simulation*/);
                    contr.Simulation.Update(stepsPerComm);
                    contr.Fitness /*= Math.Min(contr.Fitness, evaluator(_simulation));*/ 
                        += evaluator(contr.Simulation, contr);
                }
                contr.Fitness /= stepsPerContr;
                contr.SpeedFactor /= stepsPerContr;
                contr.MovementFactor /= stepsPerContr;
                contr.ProximityFactor /= stepsPerContr;

                contr.Fitness = (fitness * iteration + contr.Fitness) / (iteration + 1);
                contr.SpeedFactor = (speedFactor * iteration + contr.SpeedFactor) / (iteration + 1);
                contr.MovementFactor = (movementFactor * iteration + contr.MovementFactor) / (iteration + 1);
                contr.ProximityFactor = (proximityFactor * iteration + contr.ProximityFactor) / (iteration + 1);

                //contr.Simulation = Simulation.CloneDefault();//*contr.S*/_simulation.ShuffleRobot(stepsPerComm * 10);
            });
	    }

        public Population Select(int n)
        {
            _controllers.Sort();
            Controller worst = _controllers.First();

            List<Controller> newList = new List<Controller>(_controllers.Count);
            for (int i = 0; i < _controllers.Count; i++)
            {
                List<Controller> challengeList = new List<Controller>(n);
                for (int j = 0; j < n; j++)
                    challengeList.Add(_controllers[random.Next(_controllers.Count)]);
                challengeList.Sort();
                Controller best = challengeList.Last();
                NeuralNetwork newNetwork = NNFactory.CreateElmanNN(best.Simulation.SensorStates.Count, 2);
                newNetwork.SetAllWeights(best.NeuralNetwork.GetAllWeights());
                if (gaMode == GA_CONST_START)
                    newList.Add(new Controller(newNetwork));
                else if (gaMode == GA_HYBRID)
                    newList.Add(new Controller(newNetwork, worst.Simulation));
            }

            return new Population(newList, iteration + 1){ gaMode = gaMode };
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
                    for (int j = flipPoint; j < weightsA.Count; j++)
                    {
                        double temp = weightsA[j];
                        weightsA[j] = weightsB[j];
                        weightsB[j] = temp;
                    }
                    _controllers[2 * i].NeuralNetwork.SetAllWeights(weightsA);
                    _controllers[2 * i + 1].NeuralNetwork.SetAllWeights(weightsB);
                    if (gaMode == GA_HYBRID)
                    {
                        double oldFitnessA = _controllers[2 * i].Fitness;
                        double oldFitnessB = _controllers[2 * i + 1].Fitness;
                        double crossCoeff = flipPoint / (double)weightsA.Count;
                        _controllers[2 * i].Fitness = crossCoeff * oldFitnessB + (1 - crossCoeff) * oldFitnessA;
                        _controllers[2 * i + 1].Fitness = (1 - crossCoeff) * oldFitnessB + crossCoeff * oldFitnessA;
                    }
                }
        }

        public void Mutate(double p)
	    {
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
