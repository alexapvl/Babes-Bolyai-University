import re
from random import randint, choice
from colorama import Fore, Style

class Discipline:
    def __init__(self, discipline_id: str, name: str):
        self.discipline_id = discipline_id
        self.name = name

    @property
    def discipline_id(self):
        return self.__discipline_id

    @discipline_id.setter
    def discipline_id(self, discipline_id: str):
        if not self.validate_id(discipline_id):
            raise ValueError("Invalid id, must contain only digits")
        self.__discipline_id = discipline_id

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, name: str):
        if not self.validate_name(name):
            raise ValueError("Invalid name, must start with a capital letter and contain only letters")
        self.__name = name

    @classmethod
    def validate_id(cls, discipline_id: str):
        if re.match("^[0-9]+$", discipline_id):
            return True
        return False

    @classmethod
    def validate_name(cls, name: str):
        """
        Validates a name using regex
        :param name: Name to be validated
        :return: True if the name is valid, False otherwise
        """
        if re.match("^[A-Z]([a-z]| |[A-Z])+[a-z]$", name):
            return True
        return False

    @classmethod
    def random_discipline(cls):
        """
        Generates a random discipline
        :return: The random discipline
        """
        disciplines = [
            # Computer Science Faculty Disciplines
            'Artificial Intelligence', 'Data Structures', 'Algorithms and Complexity', 'Computer Networks',
            'Database Management Systems', 'Software Engineering', 'Machine Learning', 'Cybersecurity', 'Operating Systems',
            'Human Computer Interaction', 'Computer Graphics', 'Natural Language Processing', 'Distributed Systems', 'Cloud Computing',
            'Mobile App Development', 'Web Development', 'Computer Vision', 'Robotics', 'Cryptography', 'Internet of Things',
            'Game Development', 'Quantum Computing', 'Big Data Analytics', 'Virtual Reality', 'Compiler Design',
            'Parallel Computing', 'Embedded Systems', 'Bioinformatics', 'Augmented Reality', 'Network Security',
            'Information Retrieval', 'Blockchain Technology', 'Computational Linguistics',
            'Software Testing and Quality Assurance', 'Computer Ethics', 'Data Mining', 'Wireless Communication', 'Computer Forensics', 'Cloud Security',
            'Computer Architecture', 'Bioinformatics', 'Ecommerce Systems', 'Natural Computing',
            'Digital Signal Processing', 'Computer Animation', 'Quantum Information Science', 'Social Network Analysis', 'Bio inspired Computing',
            'Fuzzy Logic', 'Cognitive Computing', 'Swarm Intelligence', 'Human Robot Interaction',
            'Evolutionary Algorithms', 'Computational Neuroscience', 'Ethics in Computing', 'Systems Biology', 'Information Theory',
            'Parallel Algorithms', 'Recommender Systems', 'Formal Methods in Software Engineering', 'Green Computing', 'Pervasive Computing',
            'Information Security Management', 'Knowledge Representation and Reasoning', 'Semantic Web',
            'Wearable Computing', 'Computational Geometry', 'Social Computing', 'Predictive Analytics', 'Quantum Machine Learning',
            'Information Visualization', 'Computational Chemistry', 'Cyber Physical Systems', 'Usability Engineering', 'Human Centered Design',
            # Business Faculty Disciplines
            'Accounting Principles', 'Marketing Management', 'Financial Management', 'Organizational Behavior',
            'Operations Management', 'Business Ethics', 'Strategic Management', 'Business Law', 'International Business',
            'Human Resource Management', 'Entrepreneurship', 'Managerial Economics', 'Corporate Finance', 'Business Analytics',
            'Supply Chain Management', 'Business Communication', 'Project Management', 'Risk Management', 'Business Intelligence',
            'Negotiation Skills', 'Leadership and Team Management', 'Consumer Behavior', 'Corporate Governance', 'Change Management',
            'Business Research Methods', 'Marketing Research', 'Brand Management', 'Digital Marketing', 'Consumer Psychology',
            'Social Media Marketing', 'Advertising and Promotion', 'Public Relations', 'Retail Marketing', 'Marketing Strategy',
            'E commerce Marketing', 'Event Marketing', 'Product Development and Management', 'Market Segmentation', 'International Marketing',
            'Pricing Strategy', 'Services Marketing', 'Influencer Marketing', 'Cross Cultural Marketing', 'Sustainable Marketing',
            'Marketing Analytics', 'Relationship Marketing', 'Mobile Marketing', 'Back to Back Marketing', 'Sports Marketing', 'Cultural Marketing',
            'Experiential Marketing', 'Cause Marketing', 'Guerrilla Marketing', 'Neuromarketing', 'Viral Marketing',
            'Green Marketing', 'Word of Mouth Marketing', 'Affiliate Marketing', 'Content Marketing',
            'Search Engine Marketing', 'Customer Relationship Management', 'Direct Marketing', 'Gamification in Marketing',
            'Influencer Relationship Management', 'Augmented Reality in Marketing', 'Customer Loyalty Programs', 'Personalization in Marketing',
            'Storytelling in Marketing', 'Emotional Branding', 'Omnichannel Marketing', 'Interactive Marketing',
            'Neuro Linguistic Programming in Marketing', 'Reputation Management', 'Social Responsibility in Marketing', 'Marketing Automation'
        ]
        return Discipline(str(randint(100, 999)), choice(disciplines))

    def update_discipline(self, new_discipline):
        """
        Updates a discipline name with the name of another discipline
        :param new_discipline: The new discipline
        :return:
        """
        if not isinstance(new_discipline, Discipline):
            raise ValueError("Invalid discipline, must be of type Discipline")
        self.name = new_discipline.name

    def __str__(self):
        return "ID: " + self.discipline_id + " | Name: " + self.name

    def __eq__(self, other):
        """
        Two disciplines are equal if they have the same id
        :return: True if the disciplines are equal, False otherwise
        """
        if not isinstance(other, Discipline):
            raise ValueError("Can't compare Discipline with other type")
        return self.discipline_id == other.discipline_id
