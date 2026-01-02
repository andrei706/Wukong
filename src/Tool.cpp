
#include "Tool.h"

Tool::Tool(const std::string &name_, float Damage_, float Cooldown_, float Range_, int Critical_Chance_, int Burst_Count_, float Burst_Delay_)
    : Name(name_), Damage(Damage_), Cooldown(Cooldown_), Range(Range_), Critical_Chance(Critical_Chance_),
Burst_Count(Burst_Count_), Burst_Delay(Burst_Delay_){
    if (Damage < 0) throw InvalidDataException(Name + " Tool Damage", Damage);
    if (Cooldown < 0) throw InvalidDataException(Name + " Tool Cooldown", Cooldown);
    if (Range < 0) throw InvalidDataException(Name + " Tool Range", Range);
    if (Burst_Count < 1) throw InvalidDataException(Name + " Tool Burst Count", Burst_Count);
}

float Tool::DamageCalculation() const {
    Random_Value_Generator<unsigned int> ChanceGenerator(1, 100);
    unsigned int Chance = ChanceGenerator.generate();
    float CalculatedDamage = Damage + Bonus_Damage;
    if (Chance < Critical_Chance) {

        return (float)(CalculatedDamage + 0.5 * CalculatedDamage) * Damage_Multiplier;
    }
    return CalculatedDamage * Damage_Multiplier;
}

float Tool::Attack(const sf::RectangleShape &Sprite, sf::Angle Degrees){
    float angleDegrees = (float)Degrees.asDegrees();
    float angleRadians = angleDegrees * (3.14 / 180.0);

    sf::Vector2f Offset{
        Range * std::cos(angleRadians),
        Range * std::sin(angleRadians)
    };

    CreateAttackHitbox(Sprite.getPosition(), Offset, Degrees);

    if (Burst_Count > 1) {
        m_attacksQueued = Burst_Count - 1;
        m_burstTimer = 0.0f;
        m_lastAngle = Degrees;
        m_sourceSprite = &Sprite;
    }

    return Cooldown;
}

void Tool::AddStat(const std::string &StatName, float Value) {
    if (StatName == "Damage") {
        Bonus_Damage += Value;
    }
    else if (StatName == "Damage_Multiplier") {
        Damage_Multiplier += Value;
    }
    else
        std::cerr << "Warning: Stat not found\n";
}

void Tool::ResetStats() {
    Bonus_Damage = 0;
    Damage_Multiplier = 1;
}

float Tool::GetCooldown() const {
    return Cooldown;
}

std::shared_ptr<Tool> Tool::clone() const {
    return std::make_shared<Tool>(*this);
}

void Tool::Update(float deltaTime) {

    if (m_attacksQueued > 0 && m_sourceSprite != nullptr) {
        m_burstTimer += deltaTime;

        if (m_burstTimer >= Burst_Delay) {
            float angleRadians = m_lastAngle.asDegrees() * (3.14159f / 180.0f);
            sf::Vector2f Offset{ Range * std::cos(angleRadians), Range * std::sin(angleRadians) };

            CreateAttackHitbox(m_sourceSprite->getPosition(), Offset, m_lastAngle);

            m_attacksQueued--;
            m_burstTimer = 0.0f;
        }
    }

    for (auto it = Attacks.begin(); it != Attacks.end(); ) {
        auto& attack = *it;
        attack->Update(deltaTime);

        if (!attack->IsActive()) {
            it = Attacks.erase(it);
        } else {
            ++it;
        }
    }
}

void Tool::ClearAttackHitboxes() {
    Attacks.clear();
}

void Tool::CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) {
    auto new_hitbox = std::make_shared<Attack_Hitbox>(
        DamageCalculation(),
        sf::Vector2f{30.0f, 100.0f},
        Position + Offset,
        Degrees,
        Cooldown
    );

    Attacks.push_back(new_hitbox);
}

void Tool::DisplayInfo(std::ostream &out) const {
    out<< std::endl<< "-------------" <<std::endl << "Weapon Name: " << Name << std::endl
            << "Damage: " << Damage << std::endl
            << "Cooldown: " << Cooldown << std::endl
            << "Range: " << Range << std::endl;
}

const std::string & Tool::GetName() {
    return Name;
}

const std::vector<std::shared_ptr<Attack_Hitbox>> & Tool::GetAttackHitboxes() const {
    return Attacks;
}

void Tool::ShowHitboxes(sf::RenderWindow &window) const {
    for (const auto & i : Attacks) {
        i->ShowSprite(window);
    }
}

std::ostream & operator<<(std::ostream &out, const Tool &object) {
    object.DisplayInfo(out);
    // for (const auto & i : object.Attacks) {
    //     out<<i;
    // }
    return out;
}
