import sys
import unittest

sys.path.append('./code')
sys.path.append('./build/lib.macosx-10.6-intel-2.7')

from isa import ISA
from numpy.random import randn

class Tests(unittest.TestCase):
	def test_default_parameters(self):
		# make sure default_parameters() works as expected
		isa = ISA(2, 4)
		params = isa.default_parameters()

		self.assertTrue(isinstance(params, dict))



	def test_train(self):
		# make sure train() doesn't throw any errors
		isa = ISA(2)
		params = isa.default_parameters()
		params['max_iter'] = 1
		params['training_method'] = 'SGD'
		params['SGD']['max_iter'] = 1
		params['SGD']['batch_size'] = 57
		isa.train(randn(2, 1000), params)



	def test_sample(self):
		isa = ISA(3, 4)

		samples = isa.sample(100)
		samples_prior = isa.sample_prior(100)

		# simple sanity checks
		self.assertEqual(samples.shape[0], isa.num_visibles)
		self.assertEqual(samples.shape[1], 100)
		self.assertEqual(samples_prior.shape[0], isa.num_hiddens)
		self.assertEqual(samples_prior.shape[1], 100)



	def test_prior_energy_gradient(self):
		isa = ISA(4)

		samples = isa.sample_prior(100)
		grad = isa.prior_energy_gradient(samples)

		# simple sanity checks
		self.assertEqual(grad.shape[0], samples.shape[0])
		self.assertEqual(grad.shape[1], samples.shape[1])


if __name__ == '__main__':
	unittest.main()
