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

        public Population(int popSize)
        {
            _controllers = new List<Controller>(popSize);
            for (int i = 0; i < popSize; i++)
                _controllers.Add(null/*.generate())*/);
        }

        public Population(List<Controller> controllers)
        {
            _controllers = controllers;
        }

        public void Evaluate(Func<Simulation, double> evaluator, int steps)
	    {
            foreach (Controller contr in _controllers)
            {
                Simulation simulation = Simulation.CloneDefault();
                contr.Fitness = 0;
                for (int i = 0; i < steps; i++)
                {
                    simulation.Update();
                    contr.Fitness += evaluator(simulation);
                    contr.MoveRobot(simulation);
                }
                contr.Fitness /= steps;
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
                newList.Add(challengeList.OrderBy(contr => contr.Fitness).Last());
            }

            return new Population(newList);
        }

        public void Crossover(double p)
        {
            // to do
        }

        public void Mutate(double p)
	    {
		    foreach(Controller contr in _controllers)
                ;//to do;;; contr.mutate(p);
	    }
    }
}
